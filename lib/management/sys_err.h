/**
 * @file sys_err.h
 * @author jake-is-ESD-protected
 * @date 2024-03-13
 * @brief System wide error types.
 *
 * 
 */

#ifndef _SYS_ERR_H_
#define _SYS_ERR_H_

typedef enum{
    e_err_no_err,
    e_err_mem_null,
    e_err_unavailable,
    e_err_bus_fault,
    e_err_file_io,
} e_err_t;

#endif // _SYS_ERR_H_