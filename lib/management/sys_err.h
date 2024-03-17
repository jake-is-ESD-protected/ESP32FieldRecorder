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
    e_syserr_no_err,
    e_syserr_mem_null,
    e_syserr_unavailable,
    e_syserr_bus_fault,
    e_syserr_param,
    e_syserr_install,
    e_syserr_file_io,
    e_syserr_jes
} e_err_t;

void sys_err_handler(void);

#endif // _SYS_ERR_H_