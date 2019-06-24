#pragma once

#include <exception>
#include <stdexcept>
#include <thread>

#include "YLGaitLib/YLGaitLib.h"

namespace ylgaitlib {

using status = ylgaitlib_status_t;

class exception final : public std::exception {
public:
    explicit exception(status status) : status_ {status} {}
    [[nodiscard]] status status() const { return status_; }
private:
    ylgaitlib::status status_;
};

}

#define YLGAITLIB_EXCEPTION_TO_STATUS_TRY                                      \
    try {
#define YLGAITLIB_EXCEPTION_TO_STATUS_CATCH_ALL                                \
    } catch (const ylgaitlib::exception &e) {                                  \
        return e.status();                                                     \
    } catch (const std::invalid_argument &) {                                  \
        return ylgaitlib_invalid_argument;                                     \
    } catch (const std::domain_error &) {                                      \
        return ylgaitlib_domain_error;                                         \
    } catch (const std::length_error &) {                                      \
        return ylgaitlib_length_error;                                         \
    } catch (const std::out_of_range &) {                                      \
        return ylgaitlib_out_of_range;                                         \
    } catch (const std::logic_error &) {                                       \
        return ylgaitlib_logic_error;                                          \
    } catch (const std::range_error &) {                                       \
        return ylgaitlib_range_error;                                          \
    } catch (const std::overflow_error &) {                                    \
        return ylgaitlib_overflow_error;                                       \
    } catch (const std::underflow_error &) {                                   \
        return ylgaitlib_underflow_error;                                      \
    } catch (const std::runtime_error &) {                                     \
        return ylgaitlib_runtime_error;                                        \
    } catch (const std::bad_typeid &) {                                        \
        return ylgaitlib_bad_typeid;                                           \
    } catch (const std::bad_cast &) {                                          \
        return ylgaitlib_bad_cast;                                             \
    } catch (const std::bad_alloc &) {                                         \
        return ylgaitlib_bad_alloc;                                            \
    } catch (const std::bad_exception &) {                                     \
        return ylgaitlib_bad_exception;                                        \
    } catch (const std::exception &) {                                         \
        return ylgaitlib_exception;                                            \
    } catch (...) {                                                            \
        return ylgaitlib_unexpected_exception;                                 \
    }
