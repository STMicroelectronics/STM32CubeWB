/**
 ******************************************************************************
  * File Name          : App/app_zigbee.c
  * Description        : Zigbee Application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "app_common.h"
#include "app_entry.h"
#include "dbg_trace.h"
#include "app_zigbee.h"
#include "shci.h"
#include "stm_logging.h"
#include "app_conf.h"
#include "stm32wbxx_core_interface_def.h"
#include "zigbee_types.h"
#include "zigbee_interface.h"
#include "stm32_seq.h"
#include "zcl/zcl.h"
#include "zcl/zcl.onoff.h"
#include "zcl/zcl.identify.h"

/* Spec is -40 dBm, but use -60 for ease of testing, and until we get real RSSI from STMAC. */
#define APP_TOUCHLINK_MIN_RSSI              -60

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               3000U

#define SW1_ENDPOINT            17
#define SW2_ENDPOINT            18
#define TOUCHLINK_ENDPOINT      200 /* arbitrary */

#define SW1_GROUP_ADDR          0x0001
#define SW2_GROUP_ADDR          0x0002

#define SW1_FLAG                0x01
#define SW2_FLAG                0x02

#define SW1_LED                 LED_RED
#define SW2_LED                 LED_GREEN
#define SW3_LED                 LED_BLUE

#define CHANNEL                 12
/* Private function prototypes -----------------------------------------------*/

/* Private variables -----------------------------------------------*/
struct zigbee_demo_info {
    bool has_init;
    struct ZigBeeT *zb;
    enum ZbStartType startupControl;
    enum ZbStatusCodeT join_status;
    uint32_t join_delay;
    bool init_after_join;

    struct ZbZclClusterT *onoff_server_1;
    struct ZbZclClusterT *onoff_client_1;
    struct ZbZclClusterT *identify_server_1;

    struct ZbZclClusterT *onoff_server_2;
    struct ZbZclClusterT *onoff_client_2;

    uint8_t sw_flags;
};

static struct zigbee_demo_info zigbee_demo_info;

static enum ZclStatusCodeT onoff_server_attr_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrCbInfoT *cb);

static const struct ZbZclAttrT onoff_server_attr_list[] = {
    /* OnOff Server Attributes */
    {
        ZCL_ONOFF_ATTR_ONOFF, ZCL_DATATYPE_BOOLEAN,
        ZCL_ATTR_FLAG_REPORTABLE | ZCL_ATTR_FLAG_PERSISTABLE | ZCL_ATTR_FLAG_DEFAULTABLE | ZCL_ATTR_FLAG_CB_NOTIFY, 0,
        onoff_server_attr_cb, {0, 0}, {0, 0}
    },
};

/* Functions Definition ------------------------------------------------------*/

static enum ZclStatusCodeT onoff_server_attr_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrCbInfoT *cb)
{
    uint8_t endpoint;
    uint8_t attrVal;

    endpoint = ZbZclClusterGetEndpoint(clusterPtr);
    switch (cb->info->attributeId) {
        case ZCL_ONOFF_ATTR_ONOFF:
            if (endpoint == SW1_ENDPOINT) {
                if (ZbZclAttrRead(zigbee_demo_info.onoff_server_1, ZCL_ONOFF_ATTR_ONOFF, NULL, &attrVal, sizeof(attrVal), false) == ZCL_STATUS_SUCCESS) {
                    if (attrVal != 0) {
                        APP_DBG("SW1_LED ON");
                        BSP_LED_On(SW1_LED);
                    }
                    else {
                        APP_DBG("SW1_LED OFF");
                        BSP_LED_Off(SW1_LED);
                    }
                }
            }
            else if (endpoint == SW2_ENDPOINT) {
                if (ZbZclAttrRead(zigbee_demo_info.onoff_server_2, ZCL_ONOFF_ATTR_ONOFF, NULL, &attrVal, sizeof(attrVal), false) == ZCL_STATUS_SUCCESS) {
                    if (attrVal != 0) {
                        APP_DBG("SW2_LED ON");
                        BSP_LED_On(SW2_LED);
                    }
                    else {
                        APP_DBG("SW2_LED OFF");
                        BSP_LED_Off(SW2_LED);
                    }
                }
            }
            break;

        default:
            break;
    } /* switch */
    return ZCL_STATUS_SUCCESS;
} /* onoff_server_attr_notify */

static void config_endpoints(void)
{
    ZbApsmeAddEndpointReqT req;
    ZbApsmeAddEndpointConfT conf;

    memset(&req, 0, sizeof(req));
    req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
    req.deviceId = ZCL_DEVICE_ONOFF_SWITCH;

    /* Endpoint: SW1_ENDPOINT */
    req.endpoint = SW1_ENDPOINT;
    ZbZclAddEndpoint(zigbee_demo_info.zb, &req, &conf);
    assert(conf.status == ZB_STATUS_SUCCESS);
    /* OnOff Server */
    zigbee_demo_info.onoff_server_1 = ZbZclOnOffServerAlloc(zigbee_demo_info.zb, SW1_ENDPOINT, NULL);
    assert(zigbee_demo_info.onoff_server_1 != NULL);
    ZbZclAttrAppendList(zigbee_demo_info.onoff_server_1, onoff_server_attr_list, ZCL_ATTR_LIST_LEN(onoff_server_attr_list));
    ZbZclClusterEndpointRegister(zigbee_demo_info.onoff_server_1);
    /* OnOff Client */
    zigbee_demo_info.onoff_client_1 = ZbZclOnOffClientAlloc(zigbee_demo_info.zb, SW1_ENDPOINT);
    assert(zigbee_demo_info.onoff_client_1 != NULL);
    ZbZclClusterEndpointRegister(zigbee_demo_info.onoff_client_1);
    /* Identify Server for Touchlink */
    zigbee_demo_info.identify_server_1 = ZbZclIdentifyServerAlloc(zigbee_demo_info.zb, SW1_ENDPOINT, NULL);
    assert(zigbee_demo_info.identify_server_1 != NULL);

    /* Endpoint: SW2_ENDPOINT */
    req.endpoint = SW2_ENDPOINT;
    ZbZclAddEndpoint(zigbee_demo_info.zb, &req, &conf);
    assert(conf.status == ZB_STATUS_SUCCESS);
    /* OnOff Server */
    zigbee_demo_info.onoff_server_2 = ZbZclOnOffServerAlloc(zigbee_demo_info.zb, SW2_ENDPOINT, NULL);
    assert(zigbee_demo_info.onoff_server_2 != NULL);
    ZbZclAttrAppendList(zigbee_demo_info.onoff_server_2, onoff_server_attr_list, ZCL_ATTR_LIST_LEN(onoff_server_attr_list));
    ZbZclClusterEndpointRegister(zigbee_demo_info.onoff_server_2);
    /* OnOff Client */
    zigbee_demo_info.onoff_client_2 = ZbZclOnOffClientAlloc(zigbee_demo_info.zb, SW2_ENDPOINT);
    assert(zigbee_demo_info.onoff_client_2 != NULL);
    ZbZclClusterEndpointRegister(zigbee_demo_info.onoff_client_2);
} /* config_endpoints */

static void config_group_addr(void)
{
    ZbApsmeAddGroupReqT req;
    ZbApsmeAddGroupConfT conf;

    memset(&req, 0, sizeof(req));
    req.endpt = SW1_ENDPOINT;
    req.groupAddr = SW1_GROUP_ADDR;
    ZbApsmeAddGroupReq(zigbee_demo_info.zb, &req, &conf);

    req.endpt = SW2_ENDPOINT;
    req.groupAddr = SW2_GROUP_ADDR;
    ZbApsmeAddGroupReq(zigbee_demo_info.zb, &req, &conf);
} /* config_group_addr */

/**
 * @brief Zigbee Demo Task
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_Demo(void)
{
    /*
     * Handle Initialization
     */
    if (!zigbee_demo_info.has_init) {
        zigbee_demo_info.sw_flags = 0;

        zigbee_demo_info.zb = ZbInit(0U, NULL, NULL);
        assert(zigbee_demo_info.zb != NULL);

        /* Create the endpoint and cluster(s) */
        config_endpoints();

        BSP_LED_Off(SW1_LED);
        BSP_LED_Off(SW2_LED);
        BSP_LED_Off(SW3_LED);

        /* Configure the joining parameters */
        zigbee_demo_info.join_status = 0x01; /* init to error status */
        zigbee_demo_info.join_delay = HAL_GetTick(); /* now */
        zigbee_demo_info.startupControl = ZbStartTypeJoin;

        /* Initialization Complete */
        zigbee_demo_info.has_init = true;
    }

    /*
     * Handle Network Joining / Forming
     */
    if ((zigbee_demo_info.join_status != ZB_STATUS_SUCCESS) && (HAL_GetTick() >= zigbee_demo_info.join_delay)) {
        struct ZbStartupT config;
        enum ZbStatusCodeT status;

        /* Configure Zigbee Logging (only need to do this once, but this is a good place to put it) */
        ZbSetLogging(zigbee_demo_info.zb, ZB_LOG_MASK_LEVEL_5, NULL);

        /* Attempt to join a zigbee network */
        ZbStartupConfigGetProDefaults(&config);

        APP_DBG("Network config : APP_STARTUP_DISTRIB");
        /* Set the TC address to be distributed. */
        config.security.trustCenterAddress = ZB_DISTRIBUTED_TC_ADDR;

        /* Using the Uncertified Distributed Global Key (d0:d1:d2:d3:d4:d5:d6:d7:d8:d9:da:db:dc:dd:de:df) */
        memcpy(config.security.distributedGlobalKey, sec_key_distrib_uncert, ZB_SEC_KEYSIZE);

        config.startupControl = zigbee_demo_info.startupControl;

        config.channelList.count = 1;
        config.channelList.list[0].page = 0;
        config.channelList.list[0].channelMask = 1 << CHANNEL; /* Channel in use*/

        /* Using ZbStartupWait (blocking) here instead of ZbStartup, in order to demonstrate how to do
         * a blocking call on the M4. */
        status = ZbStartupWait(zigbee_demo_info.zb, &config);

        APP_DBG("ZbStartup Callback (status = 0x%02x)", status);
        zigbee_demo_info.join_status = status;

        if (status == ZB_STATUS_SUCCESS) {
            zigbee_demo_info.join_delay = 0U;
            zigbee_demo_info.init_after_join = true;
            BSP_LED_On(SW3_LED);
        }
        else {
            APP_DBG("Startup failed, attempting to form a network after a short delay.");
            zigbee_demo_info.startupControl = ZbStartTypeForm;

            zigbee_demo_info.join_delay = HAL_GetTick() + APP_ZIGBEE_STARTUP_FAIL_DELAY;
        }
    }

    /*
     * Handle Application (Switches and LEDs)
     */
    if (zigbee_demo_info.join_status == ZB_STATUS_SUCCESS) {
        /* Check if we just joined */
        if (zigbee_demo_info.init_after_join) {
            zigbee_demo_info.init_after_join = false;

            /* Assign ourselves to the group addresses */
            config_group_addr();

            /* Since we're using group addressing (broadcast), shorten the broadcast timeout */
            uint32_t bcast_timeout = 3;
            ZbNwkSet(zigbee_demo_info.zb, ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime, &bcast_timeout, sizeof(bcast_timeout));
        }

        /* Check for switch flags */
        if (zigbee_demo_info.sw_flags != 0) {
            struct ZbApsAddrT dst;

            memset(&dst, 0, sizeof(dst));
            dst.mode = ZB_APSDE_ADDRMODE_GROUP;

            if ((zigbee_demo_info.sw_flags & SW1_FLAG) != 0) {
                dst.endpoint = SW1_ENDPOINT;
                dst.nwkAddr = SW1_GROUP_ADDR;

                APP_DBG("SW1 PUSHED (SENDING TOGGLE TO GROUP 0x0001)");
                if (ZbZclOnOffClientToggleReq(zigbee_demo_info.onoff_client_1, &dst, NULL, NULL) != ZB_STATUS_SUCCESS) {
                    APP_DBG("Error, ZbZclOnOffClientToggleReq failed (SW1_ENDPOINT)");
                }
                /* We will get a loopback message because we belong to the same APS Group,
                 * causing our own local LED to toggle. */
            }

            if ((zigbee_demo_info.sw_flags & SW2_FLAG) != 0) {
                dst.endpoint = SW2_ENDPOINT;
                dst.nwkAddr = SW2_GROUP_ADDR;

                APP_DBG("SW2 PUSHED (SENDING TOGGLE TO GROUP 0x0002)");
                if (ZbZclOnOffClientToggleReq(zigbee_demo_info.onoff_client_2, &dst, NULL, NULL) != ZB_STATUS_SUCCESS) {
                    APP_DBG("Error, ZbZclOnOffClientToggleReq failed (SW2_ENDPOINT)");
                }
                /* We will get a loopback message because we belong to the same APS Group,
                 * causing our own local LED to toggle. */
            }

            /* Clear the flags */
            zigbee_demo_info.sw_flags = 0;
        }

    }

    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP, CFG_SCH_PRIO_0);
} /* APP_ZIGBEE_Demo */

/*************************************************************
 * ZbStartupWait Blocking Call
 *************************************************************/

struct ZbStartupWaitInfo {
    bool active;
    enum ZbStatusCodeT status;
};

static void ZbStartupWaitCb(enum ZbStatusCodeT status, void *cb_arg)
{
    struct ZbStartupWaitInfo *info = cb_arg;

    info->status = status;
    info->active = false;
} /* ZbStartupWaitCb */

enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config)
{
    struct ZbStartupWaitInfo *info;
    enum ZbStatusCodeT status;

    info = malloc(sizeof(struct ZbStartupWaitInfo));
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    memset(info, 0, sizeof(struct ZbStartupWaitInfo));

    info->active = true;
    status = ZbStartup(zb, config, ZbStartupWaitCb, info);
    if (status != ZB_STATUS_SUCCESS) {
        info->active = false;
        return status;
    }
    while (info->active) {
        UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );
    }
    status = info->status;
    free(info);
    return status;
} /* ZbStartupWait */

/*************************************************************
 * External Interrupt Handler
 *************************************************************/

/**
  * @brief This function manage the Push button action
  * @param  GPIO_Pin : GPIO pin which has been activated
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin) {
        case BUTTON_SW1_PIN:
            zigbee_demo_info.sw_flags |= SW1_FLAG;
            break;

        case BUTTON_SW2_PIN:
            zigbee_demo_info.sw_flags |= SW2_FLAG;
            break;

        case BUTTON_SW3_PIN:
            APP_DBG("SW3 PUSHED (LOCAL LEDS OFF)");
            ZbZclAttrIntegerWrite(zigbee_demo_info.onoff_server_1, ZCL_ONOFF_ATTR_ONOFF, 0);
            ZbZclAttrIntegerWrite(zigbee_demo_info.onoff_server_2, ZCL_ONOFF_ATTR_ONOFF, 0);
            break;

        default:
            break;
    }
} /* HAL_GPIO_EXTI_Callback */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
