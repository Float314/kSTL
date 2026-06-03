#pragma once

#include "kstl/types.hpp"

namespace kstd {
    struct rt_allocator {
        void* (*alloc)(size_t mem) = nullptr;
        void (*free)(void*) = nullptr;
    };

    struct rt_init {
        rt_allocator allocator_info;
        void (*panic)(void) = nullptr;
    };

    enum class error_category : int {
        null = 0,
        custom_failure,
        rt_init_failure,
        // Credits: https://cppreference.com/cpp/header/system_error
        address_family_not_supported,       // EAFNOSUPPORT
        address_in_use,                     // EADDRINUSE
        address_not_available,              // EADDRNOTAVAIL
        already_connected,                  // EISCONN
        argument_list_too_long,             // E2BIG
        argument_out_of_domain,             // EDOM
        bad_address,                        // EFAULT
        bad_file_descriptor,                // EBADF
        bad_message,                        // EBADMSG
        broken_pipe,                        // EPIPE
        connection_aborted,                 // ECONNABORTED
        connection_already_in_progress,     // EALREADY
        connection_refused,                 // ECONNREFUSED
        connection_reset,                   // ECONNRESET
        cross_device_link,                  // EXDEV
        destination_address_required,       // EDESTADDRREQ
        device_or_resource_busy,            // EBUSY
        directory_not_empty,                // ENOTEMPTY
        executable_format_error,            // ENOEXEC
        file_exists,                        // EEXIST
        file_too_large,                     // EFBIG
        filename_too_long,                  // ENAMETOOLONG
        function_not_supported,             // ENOSYS
        host_unreachable,                   // EHOSTUNREACH
        identifier_removed,                 // EIDRM
        illegal_byte_sequence,              // EILSEQ
        inappropriate_io_control_operation, // ENOTTY
        interrupted,                        // EINTR
        invalid_argument,                   // EINVAL
        invalid_seek,                       // ESPIPE
        io_error,                           // EIO
        is_a_directory,                     // EISDIR
        message_size,                       // EMSGSIZE
        network_down,                       // ENETDOWN
        network_reset,                      // ENETRESET
        network_unreachable,                // ENETUNREACH
        no_buffer_space,                    // ENOBUFS
        no_child_process,                   // ECHILD
        no_link,                            // ENOLINK
        no_lock_available,                  // ENOLCK
        no_message_available,               // ENODATA
        no_message,                         // ENOMSG
        no_protocol_option,                 // ENOPROTOOPT
        no_space_on_device,                 // ENOSPC
        no_stream_resources,                // ENOSR
        no_such_device_or_address,          // ENXIO
        no_such_device,                     // ENODEV
        no_such_file_or_directory,          // ENOENT
        no_such_process,                    // ESRCH
        not_a_directory,                    // ENOTDIR
        not_a_socket,                       // ENOTSOCK
        not_a_stream,                       // ENOSTR
        not_connected,                      // ENOTCONN
        not_enough_memory,                  // ENOMEM
        not_supported,                      // ENOTSUP
        operation_canceled,                 // ECANCELED
        operation_in_progress,              // EINPROGRESS
        operation_not_permitted,            // EPERM
        operation_not_supported,            // EOPNOTSUPP
        operation_would_block,              // EWOULDBLOCK
        owner_dead,                         // EOWNERDEAD
        permission_denied,                  // EACCES
        protocol_error,                     // EPROTO
        protocol_not_supported,             // EPROTONOSUPPORT
        read_only_file_system,              // EROFS
        resource_deadlock_would_occur,      // EDEADLK
        resource_unavailable_try_again,     // EAGAIN
        result_out_of_range,                // ERANGE
        state_not_recoverable,              // ENOTRECOVERABLE
        stream_timeout,                     // ETIME
        text_file_busy,                     // ETXTBSY
        timed_out,                          // ETIMEDOUT
        too_many_files_open_in_system,      // ENFILE
        too_many_files_open,                // EMFILE
        too_many_links,                     // EMLINK
        too_many_symbolic_link_levels,      // ELOOP
        value_too_large,                    // EOVERFLOW
        wrong_protocol_type,                // EPROTOTYPE
    };

    template<typename T>
    struct type_wrapper {
        T _value;
        operator T() const { return this->_value; }
        type_wrapper operator=(const T &value) {
            this->_value = value;
            return *this;
        }
        type_wrapper() = default;
        type_wrapper(const T &value) {
            this->_value = value;
        };
        type_wrapper(T &value) {
            this->_value = value;
        }
        ~type_wrapper() = default;
    };

    template<>
    struct type_wrapper<void> {};

    /// @note An error code for error handling
    /// @note This is the only class (along with error_category) that will not be standards-compliant
    template<typename T>
    struct error_code {
    private:
        type_wrapper<T> _value;
        error_category _category = error_category::null;
    public:
        T value() const noexcept {
            return this->_value;
        }
        const error_category& category() const noexcept {
            return this->_category;
        }
    public:
        void assign(type_wrapper<T> value, error_category category) noexcept {
            this->_value = value;
            this->_category = category;
        }
        void clear() noexcept {
            this->_category = error_category::null;
            this->_value = {};
        }
    public:
        explicit operator bool() const noexcept {
            return false ? this->_category == error_category::null
                : true;
        }
    public:
        error_code() = default;
        error_code(type_wrapper<T> value, error_category category) {
            this->_value = value;
            this->_category = category;
        }
        ~error_code() = default;
    };


    template<>
    struct error_code<void> {
    private:
        type_wrapper<void> _value;
        error_category _category = error_category::null;
    public:
        void value() const noexcept = delete;
        const error_category& category() const noexcept {
            return this->_category;
        }
    public:
        void assign(type_wrapper<void> value, error_category category) noexcept {
            this->_value = value;
            this->_category = category;
        }
        void clear() noexcept {
            this->_category = error_category::null;
            this->_value = {};
        }
    public:
        explicit operator bool() const noexcept {
            return false ? this->_category == error_category::null
                : true;
        }
    public:
        error_code() = default;
        error_code(type_wrapper<void> value, error_category category) {
            this->_value = value;
            this->_category = category;
        }
        ~error_code() = default;
    };

    [[nodiscard]] error_code<void> init(const rt_init &init_data);
}
