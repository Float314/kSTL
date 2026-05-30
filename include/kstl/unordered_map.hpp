#pragma once

#include <concepts>
#include <initializer_list>
#include <new>
#include <kstl/types.hpp>
#include <kstl/stdlib.hpp>
#include <kstl/utility.hpp>
#include <kstl/runtime.hpp>
#include <kstl/algorithm.hpp>
#include <kstl/functional.hpp>
#include <kstl/vector.hpp>
#include <kstl/string.hpp>

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
    extern void* (*malloc)(size_t);
    extern void (*free)(void*);
}

namespace kstd {
    template<typename Key, typename T, class Hash = hash<Key>, class KeyEqual = equal_to<Key>>
    class unordered_map {
    private:
        struct node_t {
            size_t hash = 0;
            node_t *next = nullptr;
            pair<const Key, T> value;

            ~node_t() = default;
        };

        using bucket_t = node_t **;

        bucket_t nodes = nullptr;
        size_t bucket_count = 0;
        size_t _size = 0;
    public:
        class const_iterator {
        public:
            unordered_map *container = nullptr;
            size_t bucket_idx = 0;
            node_t *current = nullptr;
        public:
            const pair<const Key, T>& operator*() const noexcept {
                return current->value;
            }

            const pair<const Key, T>* operator->() const noexcept {
                return &current->value;
            }

            bool operator==(const const_iterator &other) const noexcept {
                return current == other.current;
            }

            bool operator!=(const const_iterator &other) const noexcept {
                return current != other.current;
            }

            void advance_to_valid() noexcept {
                while (current == nullptr && bucket_idx + 1 < container->bucket_count) {
                    ++bucket_idx;
                    this->current = container->nodes[bucket_idx];
                }
            }

            const_iterator(unordered_map *ctr, size_t bidx, node_t *n) noexcept
                : container(ctr)
                , bucket_idx(bidx)
                , current(n) 
            {
                advance_to_valid();
            }

            const_iterator() = delete;

            ~const_iterator() = default;
        };

        class iterator {
        public:
            unordered_map *container = nullptr;
            size_t bucket_idx = 0;
            node_t *current = nullptr;
        public:
            pair<const Key, T>& operator*() const noexcept {
                return current->value;
            }

            pair<const Key, T>* operator->() const noexcept {
                return &current->value;
            }

            iterator& operator++() noexcept {
                if (current != nullptr) {
                    current = current->next;
                }

                advance_to_valid();
                return *this;
            }

            bool operator==(const iterator &other) const noexcept {
                return current == other.current;
            }

            bool operator!=(const iterator &other) const noexcept {
                return current != other.current;
            }

            void advance_to_valid() noexcept {
                while (current == nullptr && bucket_idx + 1 < container->bucket_count) {
                    ++bucket_idx;
                    this->current = container->nodes[bucket_idx];
                }
            }

            iterator(unordered_map *ctr, size_t bidx, node_t *n) noexcept
                : container(ctr)
                , bucket_idx(bidx)
                , current(n) 
            {
                advance_to_valid();
            }

            operator class const_iterator() {
                return { container, bucket_idx, current };
            }

            iterator() = delete;
        public:
            ~iterator() = default;
        };
    private:
        [[no_unique_address]] KeyEqual comparator;
        [[no_unique_address]] Hash hasher;
    private:
        void grow() {
            size_t old_bucket_count = this->bucket_count;
            this->bucket_count *= 2;
            node_t **new_nodes = reinterpret_cast<bucket_t>(kstl_globals::malloc(sizeof(node_t*) * this->bucket_count));
            kstd::memset(new_nodes, 0, sizeof(node_t *) * this->bucket_count);
            node_t **old_nodes = this->nodes;
            this->nodes = new_nodes;

            for (size_t i = 0; i < old_bucket_count; ++i) {
                node_t *node = old_nodes[i];

                while (node != nullptr) {
                    node_t *next_node = node->next;

                    size_t idx = node->hash % this->bucket_count;
                    node->next = this->nodes[idx];
                    this->nodes[idx] = node;

                    node = next_node;
                }
            }

            kstl_globals::free(old_nodes);
        }
    private:
        kstd::pair<iterator, bool> put(const Key &key, const T &value) {
            size_t hash = hasher(key);
            size_t idx = hash % bucket_count;

            {
                node_t *node = this->nodes[idx];
                while (node != nullptr) {
                    if (comparator(key, node->value.first)) {
                        return {end(), false};
                    }

                    node = node->next;
                }
            }

            node_t *new_node = reinterpret_cast<node_t*>(kstl_globals::malloc(sizeof(node_t)));
            new (new_node) node_t {
                .hash  = hash,
                .next  = nullptr,
                .value = { key, value }
            };

            new_node->next = this->nodes[idx];

            this->nodes[idx] = new_node;

            ++this->_size;

            if (this->_size > this->bucket_count) {
                this->grow();
            }

            return { { this, idx, new_node }, true };
        }
    public:
    public:
        size_t size() const noexcept {
            return this->_size;
        }

        bool empty() const noexcept {
            return this->_size == 0;
        }

        // TODO: Implement
        size_t max_size() const noexcept = delete;

        iterator find(const Key &key) noexcept {
            if (this->nodes != nullptr) {
                for (size_t i = 0; i < this->bucket_count; ++i) {
                    node_t *node = this->nodes[i];
                    while (node != nullptr) {
                        node_t *next_node = node->next;

                        if (comparator(node->value.first, key)) {
                            return { this, i, node };
                        }

                        node = next_node;
                    }
                }
            }

            return end();
        }

        // TODO: Implement
        // iterator find(Key &&key) noexcept = delete;

        // TODO: Implement
        // const_iterator find(Key &&key) const noexcept = delete;

        const_iterator find(const Key &key) const noexcept {
            if (this->nodes != nullptr) {
                for (size_t i = 0; i < this->bucket_count; ++i) {
                    node_t *node = this->nodes[i];
                    while (node != nullptr) {
                        node_t *next_node = node->next;

                        if (comparator(node->value.first, key)) {
                            return { this, i, node };
                        }

                        node = next_node;
                    }
                }
            }

            return end();
        }
    public:
        void clear() noexcept {
            if (this->nodes != nullptr) {
                for (size_t i = 0; i < this->bucket_count; ++i) {
                    node_t *node = this->nodes[i];
                    while (node != nullptr) {
                        node_t *next_node = node->next;

                        node->~node_t();
                        kstl_globals::free(node);

                        node = next_node;
                    }
                }
                kstl_globals::free(nodes);
            }
        }
    public:
        kstd::pair<iterator, bool> insert(const kstd::pair<const Key, T> &vt) noexcept {
            return put(vt.first, vt.second);
        }

        // TODO: Implement
        template<typename... Args>
        kstd::pair<iterator, bool> emplace(Args&&... args) noexcept = delete;
    public:
        iterator begin() noexcept {
            return { this, 0, this->nodes[0] };
        }

        iterator end() noexcept {
            return { this, this->bucket_count, nullptr };
        }

        const_iterator begin() const noexcept {
            return { this, 0, this->nodes[0] };
        }

        const_iterator end() const noexcept {
            return { this, this->bucket_count, nullptr };
        }
    public:
        void erase(iterator it) noexcept {
            if (this->nodes != nullptr && it != end()) {
                for (size_t i = 0; i < this->bucket_count; ++i) {
                    node_t *node = this->nodes[i];
                    node_t *prev = nullptr;
                    while (node != nullptr) {
                        node_t *next_node = node->next;

                        if (comparator(node->value.first, it->first)) {
                            if (prev != nullptr) {
                                prev->next = next_node;
                            } else {
                                this->nodes[i] = next_node;
                            }

                            node->~node_t();
                            kstl_globals::free(node);
                        }

                        prev = node;
                        node = next_node;
                    }
                }
            }
        }

        iterator erase(const_iterator it) noexcept {
            if (this->nodes != nullptr && it != end()) {
                for (size_t i = 0; i < this->bucket_count; ++i) {
                    node_t *node = this->nodes[i];
                    node_t *prev = nullptr;
                    while (node != nullptr) {
                        node_t *next_node = node->next;

                        if (comparator(node->value.first, it->first)) {
                            if (prev != nullptr) {
                                prev->next = next_node;
                            } else {
                                this->nodes[i] = next_node;
                            }

                            node->~node_t();
                            kstl_globals::free(node);
                        }

                        return ++it;

                        prev = node;
                        node = next_node;
                    }
                }
            }

            return end();
        }

        iterator erase(const Key &key) noexcept {
            // TODO: i'm too tired for this
            return erase(find(key));
        }

        // TODO: Implement
        iterator erase(const_iterator begin, const_iterator end) noexcept = delete;

        // iterator erase(const_iterator begin, const_iterator end) noexcept {
        //     if (this->nodes != nullptr && begin != this->end()) {
        //         for (size_t i = 0; i < this->bucket_count; ++i) {
        //             node_t *node = this->nodes[i];
        //             node_t *prev = nullptr;
        //             while (node != nullptr && ++begin != end) {
        //                 node_t *next_node = node->next;
        //
        //                 if (begin->current != node)
        //                     continue;
        //
        //                 if (prev != nullptr) {
        //                     prev->next = next_node;
        //                 } else {
        //                     this->nodes[i] = next_node;
        //                 }
        //
        //                 node->~node_t();
        //                 kstl_globals::free(node);
        //
        //                 prev = node;
        //                 node = next_node;
        //             }
        //         }
        //
        //         return begin;
        //     }
        //
        //     return end();
        // }
    public:
        T& operator[](const Key &key) noexcept {
            auto it = find(key);
            if (it != end()) {
                return it->second;
            }

            auto th = put(key, {});
            return th.first->second;
        }

        // TODO: Implement
        // How are we gonna move it but use it twice?
        // T& operator[](Key &&key) noexcept = delete;

        unordered_map& operator=(const unordered_map &other) noexcept {
            if (&other == this) return *this;

            this->clear();

            this->bucket_count = other.bucket_count;
            this->nodes = reinterpret_cast<bucket_t>(kstl_globals::malloc(sizeof(node_t*) * this->bucket_count));
            kstd::memset(this->nodes, 0, sizeof(node_t*) * this->bucket_count);

            for (size_t i = 0; i < other.bucket_count; ++i) {
                node_t *node = other.nodes[i];
                while (node != nullptr) {
                    node_t *new_node = reinterpret_cast<node_t*>(kstl_globals::malloc(sizeof(node_t)));
                    new (new_node) node_t(*node);

                    new_node->next = this->nodes[node->hash % other.bucket_count];
                    this->nodes[i] = new_node->next;

                    node = node->next;
                }
            }
        }

        unordered_map& operator=(unordered_map &&other) noexcept {
            if (&other == this) return *this;

            this->bucket_count = other.bucket_count;
            this->_size = other._size;
            this->nodes = other.nodes;

            other.bucket_count = 0;
            other._size = 0;
            other.nodes = nullptr;
        }
    public:
        unordered_map() {
            bucket_count = 8;
            nodes = reinterpret_cast<bucket_t>(kstl_globals::malloc(sizeof(node_t*) * bucket_count));
            kstd::memset(nodes, 0, sizeof(node_t *) * this->bucket_count);
        }

        unordered_map(const unordered_map &other) {
            this->bucket_count = other.bucket_count;
            this->nodes = reinterpret_cast<bucket_t>(kstl_globals::malloc(sizeof(node_t*) * this->bucket_count));
            kstd::memset(this->nodes, 0, sizeof(node_t*) * this->bucket_count);

            for (size_t i = 0; i < other.bucket_count; ++i) {
                node_t *node = other.nodes[i];
                while (node != nullptr) {
                    node_t *new_node = reinterpret_cast<node_t*>(kstl_globals::malloc(sizeof(node_t)));
                    new (new_node) node_t(*node);

                    new_node->next = this->nodes[node->hash % other.bucket_count];
                    this->nodes[i] = new_node->next;

                    node = node->next;
                }
            }
        }

        unordered_map(unordered_map &&other) {
            this->bucket_count = other.bucket_count;
            this->_size = other._size;
            this->nodes = other.nodes;

            other.bucket_count = 0;
            other._size = 0;
            other.nodes = nullptr;
        }

        ~unordered_map() {
            for (size_t i = 0; i < bucket_count; ++i) {
                node_t *head = this->nodes[i];
                while (head) {
                    node_t* next = head->next;

                    head->~node_t();
                    kstl_globals::free(head);

                    head = next;
                }
            }

            kstl_globals::free(this->nodes);
        }
    };
}
