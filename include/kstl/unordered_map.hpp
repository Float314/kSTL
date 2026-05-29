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

        node_t **nodes = nullptr;
        size_t bucket_count = 0;
        size_t _size = 0;
    private:
        [[no_unique_address]] KeyEqual comparator;
        [[no_unique_address]] Hash hasher;
    private:
        void grow() {
            size_t old_bucket_count = this->bucket_count;
            this->bucket_count *= 2;
            node_t **new_nodes = reinterpret_cast<node_t**>(kstl_globals::malloc(sizeof(node_t*) * this->bucket_count));
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
        void put(const Key &key, const T &value) {
            size_t hash = hasher(key);
            size_t idx = hash % bucket_count;

            {
                node_t *node = this->nodes[idx];
                while (node != nullptr) {
                    if (comparator(key, node->value.first)) {
                        return;
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
        }
    public:
        size_t size() const noexcept {
            return this->_size;
        }
    public:
        unordered_map& operator=(const unordered_map &other) noexcept {
            if (&other == this) return *this;

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

            this->bucket_count = other.bucket_count;
            this->nodes = reinterpret_cast<node_t**>(kstl_globals::malloc(sizeof(node_t*) * this->bucket_count));
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

        unordered_map& operator=(unordered_map &&) noexcept;
    public:
        unordered_map() {
            bucket_count = 8;
            nodes = reinterpret_cast<node_t**>(kstl_globals::malloc(sizeof(node_t*) * bucket_count));
            kstd::memset(nodes, 0, sizeof(node_t *) * this->bucket_count);
        }

        unordered_map(const unordered_map &other) {
            this->bucket_count = other.bucket_count;
            this->nodes = reinterpret_cast<node_t**>(kstl_globals::malloc(sizeof(node_t*) * this->bucket_count));
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
    
    void x() {
        unordered_map<int, float> map;
    }
}
