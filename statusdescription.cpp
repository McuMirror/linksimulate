#include "statusdescription.h"
#include <stdio.h>
#include <string>

#define MY_UNUSED(x) (void)(x)

ViStatus StatusDescription::statusDesc(ViObject vi, ViStatus status, ViChar desc[])
{
    MY_UNUSED(vi);

    bool flag = true;
    std::string info("null");

    switch (status) {
    case VI_SUCCESS_EVENT_EN:
        info.assign("VI_SUCCESS_EVENT_EN");
        break;
    case VI_SUCCESS_EVENT_DIS:
        info.assign("VI_SUCCESS_EVENT_DIS");
        break;
    case VI_SUCCESS_QUEUE_EMPTY:
        info.assign("VI_SUCCESS_QUEUE_EMPTY");
        break;
    case VI_SUCCESS_TERM_CHAR:
        info.assign("VI_SUCCESS_TERM_CHAR");
        break;
    case VI_SUCCESS_MAX_CNT:
        info.assign("VI_SUCCESS_MAX_CNT");
        break;
    case VI_SUCCESS_DEV_NPRESENT:
        info.assign("VI_SUCCESS_DEV_NPRESENT");
        break;
    case VI_SUCCESS_TRIG_MAPPED:
        info.assign("VI_SUCCESS_TRIG_MAPPED");
        break;
    case VI_SUCCESS_QUEUE_NEMPTY:
        info.assign("VI_SUCCESS_QUEUE_NEMPTY");
        break;
    case VI_SUCCESS_NCHAIN:
        info.assign("VI_SUCCESS_NCHAIN");
        break;
    case VI_SUCCESS_NESTED_SHARED:
        info.assign("VI_SUCCESS_NESTED_SHARED");
        break;
    case VI_SUCCESS_NESTED_EXCLUSIVE:
        info.assign("VI_SUCCESS_NESTED_EXCLUSIVE");
        break;
    case VI_SUCCESS_SYNC:
        info.assign("VI_SUCCESS_SYNC");
        break;

    case VI_WARN_CONFIG_NLOADED:
        info.assign("VI_WARN_CONFIG_NLOADED");
        break;
    case VI_WARN_NULL_OBJECT:
        info.assign("VI_WARN_NULL_OBJECT");
        break;
    case VI_WARN_NSUP_ATTR_STATE:
        info.assign("VI_WARN_NSUP_ATTR_STATE");
        break;
    case VI_WARN_UNKNOWN_STATUS:
        info.assign("VI_WARN_UNKNOWN_STATUS");
        break;
    case VI_WARN_NSUP_BUF:
        info.assign("VI_WARN_NSUP_BUF");
        break;

    case VI_ERROR_SYSTEM_ERROR:
        info.assign("VI_ERROR_SYSTEM_ERROR");
        break;
    case VI_ERROR_INV_OBJECT:
        info.assign("VI_ERROR_INV_OBJECT");
        break;
    case VI_ERROR_RSRC_LOCKED:
        info.assign("VI_ERROR_RSRC_LOCKED");
        break;
    case VI_ERROR_INV_EXPR:
        info.assign("VI_ERROR_INV_EXPR");
        break;
    case VI_ERROR_RSRC_NFOUND:
        info.assign("VI_ERROR_RSRC_NFOUND");
        break;
    case VI_ERROR_INV_RSRC_NAME:
        info.assign("VI_ERROR_INV_RSRC_NAME");
        break;
    case VI_ERROR_INV_ACC_MODE:
        info.assign("VI_ERROR_INV_ACC_MODE");
        break;
    case VI_ERROR_TMO:
        info.assign("VI_ERROR_TMO");
        break;
    case VI_ERROR_CLOSING_FAILED:
        info.assign("VI_ERROR_CLOSING_FAILED");
        break;
    case VI_ERROR_INV_DEGREE:
        info.assign("VI_ERROR_INV_DEGREE");
        break;
    case VI_ERROR_INV_JOB_ID:
        info.assign("VI_ERROR_INV_JOB_ID");
        break;
    case VI_ERROR_NSUP_ATTR:
        info.assign("VI_ERROR_NSUP_ATTR");
        break;
    case VI_ERROR_NSUP_ATTR_STATE:
        info.assign("VI_ERROR_NSUP_ATTR_STATE");
        break;
    case VI_ERROR_ATTR_READONLY:
        info.assign("VI_ERROR_ATTR_READONLY");
        break;
    case VI_ERROR_INV_LOCK_TYPE:
        info.assign("VI_ERROR_INV_LOCK_TYPE");
        break;
    case VI_ERROR_INV_ACCESS_KEY:
        info.assign("VI_ERROR_INV_ACCESS_KEY");
        break;
    case VI_ERROR_INV_EVENT:
        info.assign("VI_ERROR_INV_EVENT");
        break;
    case VI_ERROR_INV_MECH:
        info.assign("VI_ERROR_INV_MECH");
        break;
    case VI_ERROR_HNDLR_NINSTALLED:
        info.assign("VI_ERROR_HNDLR_NINSTALLED");
        break;
    case VI_ERROR_INV_HNDLR_REF:
        info.assign("VI_ERROR_INV_HNDLR_REF");
        break;
    case VI_ERROR_INV_CONTEXT:
        info.assign("VI_ERROR_INV_CONTEXT");
        break;
    case VI_ERROR_QUEUE_ERROR:
        info.assign("VI_ERROR_QUEUE_ERROR");
        break;
    case VI_ERROR_NENABLED:
        info.assign("VI_ERROR_NENABLED");
        break;
    case VI_ERROR_ABORT:
        info.assign("VI_ERROR_ABORT");
        break;
    case VI_ERROR_RAW_WR_PROT_VIOL:
        info.assign("VI_ERROR_RAW_WR_PROT_VIOL");
        break;
    case VI_ERROR_RAW_RD_PROT_VIOL:
        info.assign("VI_ERROR_RAW_RD_PROT_VIOL");
        break;
    case VI_ERROR_OUTP_PROT_VIOL:
        info.assign("VI_ERROR_OUTP_PROT_VIOL");
        break;
    case VI_ERROR_INP_PROT_VIOL:
        info.assign("VI_ERROR_INP_PROT_VIOL");
        break;
    case VI_ERROR_BERR:
        info.assign("VI_ERROR_BERR");
        break;
    case VI_ERROR_IN_PROGRESS:
        info.assign("VI_ERROR_IN_PROGRESS");
        break;
    case VI_ERROR_INV_SETUP:
        info.assign("VI_ERROR_INV_SETUP");
        break;
    case VI_ERROR_ALLOC:
        info.assign("VI_ERROR_ALLOC");
        break;
    case VI_ERROR_INV_MASK:
        info.assign("VI_ERROR_INV_MASK");
        break;
    case VI_ERROR_IO:
        info.assign("VI_ERROR_IO");
        break;
    case VI_ERROR_INV_FMT:
        info.assign("VI_ERROR_INV_FMT");
        break;
    case VI_ERROR_NSUP_FMT:
        info.assign("VI_ERROR_NSUP_FMT");
        break;
    case VI_ERROR_LINE_IN_USE:
        info.assign("VI_ERROR_LINE_IN_USE");
        break;
    case VI_ERROR_NSUP_MODE:
        info.assign("VI_ERROR_NSUP_MODE");
        break;
    case VI_ERROR_SRQ_NOCCURRED:
        info.assign("VI_ERROR_SRQ_NOCCURRED");
        break;
    case VI_ERROR_INV_SPACE:
        info.assign("VI_ERROR_INV_SPACE");
        break;
    case VI_ERROR_INV_OFFSET:
        info.assign("VI_ERROR_INV_OFFSET");
        break;
    case VI_ERROR_INV_WIDTH:
        info.assign("VI_ERROR_INV_WIDTH");
        break;
    case VI_ERROR_NSUP_OFFSET:
        info.assign("VI_ERROR_NSUP_OFFSET");
        break;
    case VI_ERROR_NSUP_VAR_WIDTH:
        info.assign("VI_ERROR_NSUP_VAR_WIDTH");
        break;
    case VI_ERROR_WINDOW_NMAPPED:
        info.assign("VI_ERROR_WINDOW_NMAPPED");
        break;
    case VI_ERROR_RESP_PENDING:
        info.assign("VI_ERROR_RESP_PENDING");
        break;
    case VI_ERROR_NLISTENERS:
        info.assign("VI_ERROR_NLISTENERS");
        break;
    case VI_ERROR_NCIC:
        info.assign("VI_ERROR_NCIC");
        break;
    case VI_ERROR_NSYS_CNTLR:
        info.assign("VI_ERROR_NSYS_CNTLR");
        break;
    case VI_ERROR_NSUP_OPER:
        info.assign("VI_ERROR_NSUP_OPER");
        break;
    case VI_ERROR_INTR_PENDING:
        info.assign("VI_ERROR_INTR_PENDING");
        break;
    case VI_ERROR_ASRL_PARITY:
        info.assign("VI_ERROR_ASRL_PARITY");
        break;
    case VI_ERROR_ASRL_FRAMING:
        info.assign("VI_ERROR_ASRL_FRAMING");
        break;
    case VI_ERROR_ASRL_OVERRUN:
        info.assign("VI_ERROR_ASRL_OVERRUN");
        break;
    case VI_ERROR_TRIG_NMAPPED:
        info.assign("VI_ERROR_TRIG_NMAPPED");
        break;
    case VI_ERROR_NSUP_ALIGN_OFFSET:
        info.assign("VI_ERROR_NSUP_ALIGN_OFFSET");
        break;
    case VI_ERROR_USER_BUF:
        info.assign("VI_ERROR_USER_BUF");
        break;
    case VI_ERROR_RSRC_BUSY:
        info.assign("VI_ERROR_RSRC_BUSY");
        break;
    case VI_ERROR_NSUP_WIDTH:
        info.assign("VI_ERROR_NSUP_WIDTH");
        break;
    case VI_ERROR_INV_PARAMETER:
        info.assign("VI_ERROR_INV_PARAMETER");
        break;
    case VI_ERROR_INV_PROT:
        info.assign("VI_ERROR_INV_PROT");
        break;
    case VI_ERROR_INV_SIZE:
        info.assign("VI_ERROR_INV_SIZE");
        break;
    case VI_ERROR_WINDOW_MAPPED:
        info.assign("VI_ERROR_WINDOW_MAPPED");
        break;
    case VI_ERROR_NIMPL_OPER:
        info.assign("VI_ERROR_NIMPL_OPER");
        break;
    case VI_ERROR_INV_LENGTH:
        info.assign("VI_ERROR_INV_LENGTH");
        break;
    case VI_ERROR_INV_MODE:
        info.assign("VI_ERROR_INV_MODE");
        break;
    case VI_ERROR_SESN_NLOCKED:
        info.assign("VI_ERROR_SESN_NLOCKED");
        break;
    case VI_ERROR_MEM_NSHARED:
        info.assign("VI_ERROR_MEM_NSHARED");
        break;
    case VI_ERROR_LIBRARY_NFOUND:
        info.assign("VI_ERROR_LIBRARY_NFOUND");
        break;
    case VI_ERROR_NSUP_INTR:
        info.assign("VI_ERROR_NSUP_INTR");
        break;
    case VI_ERROR_INV_LINE:
        info.assign("VI_ERROR_INV_LINE");
        break;
    case VI_ERROR_FILE_ACCESS:
        info.assign("VI_ERROR_FILE_ACCESS");
        break;
    case VI_ERROR_FILE_IO:
        info.assign("VI_ERROR_FILE_IO");
        break;
    case VI_ERROR_NSUP_LINE:
        info.assign("VI_ERROR_NSUP_LINE");
        break;
    case VI_ERROR_NSUP_MECH:
        info.assign("VI_ERROR_NSUP_MECH");
        break;
    case VI_ERROR_INTF_NUM_NCONFIG:
        info.assign("VI_ERROR_INTF_NUM_NCONFIG");
        break;
    case VI_ERROR_CONN_LOST:
        info.assign("VI_ERROR_CONN_LOST");
        break;
    default:
        flag = false;
        info.assign("VI_WARN_UNKNOWN_STATUS");
        break;
    }

    strcpy_s(desc, BUFFER_SIZE, info.c_str());

    if(flag)
        return VI_SUCCESS;
    else
        return VI_WARN_UNKNOWN_STATUS;
}

StatusDescription::StatusDescription()
{

}



