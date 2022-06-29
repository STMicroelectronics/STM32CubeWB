
/**
  ******************************************************************************
 * @file    ancs_client_app.h
 * @author  MCD Application Team
 * @brief   Header for ancs_client_app.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018(-2021) STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ANCS_APPLICATION_H
#define ANCS_APPLICATION_H

#ifdef __cplusplus
extern "C" {
#endif
/**
 *@addtogroup ANCS ANCS
 *@brief ANCS Profile.
 *@{
 */

/**
 *@defgroup ANCS_defines ANCS defines
 *@brief List of constant used in the ANCS.
 *@{
 */

/**
 * @anchor CategoryIDs
 * @name Category ID values
 * @{
 */
typedef enum {
  CategoryIDOther = 0,
  CategoryIDIncomingCall = 1,
  CategoryIDMissedCall = 2,
  CategoryIDVoicemail = 3,
  CategoryIDSocial = 4,
  CategoryIDSchedule = 5,
  CategoryIDEmail = 6,
  CategoryIDNews = 7,
  CategoryIDHealthAndFitness = 8,
  CategoryIDBusinessAndFinance = 9,
  CategoryIDLocation = 10,
  CategoryIDEntertainment = 11,
} CategoryID;
/**
 * @}
 */

/**
 * @anchor EventIDs
 * @name Event ID values
 * @{
 */
typedef enum {
  EventIDNotificationAdded = 0,
  EventIDNotificationModified = 1,
  EventIDNotificationRemoved = 2,
} EventID;
/**
 * @}
 */

/**
 * @anchor EventFlags
 * @name Event Flag values
 * @{
 */
typedef enum {
  EventFlagSilent = (1 << 0),
  EventFlagImportant = (1 << 1),
  EventFlagPreExisting = (1 << 2),
  EventFlagPositiveAction = (1 << 3),
  EventFlagNegativeAction = (1 << 4),
} EventFlags;
/**
 * @}
 */

/**
 * @anchor CommandIDs to control point characteristic commands
 * @name Command ID values
 * @{
 */
typedef enum {
  CommandIDGetNotificationAttributes = 0,
  CommandIDGetAppAttributes = 1,
  CommandIDPerformNotificationAction = 2,
} CommandID;
/**
 * @}
 */

/**
 * @anchor NotificationAttributeIDs for Get Notification Attribute command
 * @name Notification Attribute ID values for Get notifications attributes command
 * @{
 */
typedef enum {
  NotificationAttributeIDAppIdentifier = 0,
  NotificationAttributeIDTitle = 1, 
  NotificationAttributeIDSubtitle = 2,
  NotificationAttributeIDMessage = 3,
  NotificationAttributeIDMessageSize = 4,
  NotificationAttributeIDDate = 5,
  NotificationAttributeIDPositiveActionLabel = 6,
  NotificationAttributeIDNegativeActionLabel = 7,
} NotificationAttributeID;
/**
 * @}
 */

/**
 * @anchor ActionIDs
 * @name Action ID values
 * @{
 */
typedef enum {
  ActionIDPositive = 0,
  ActionIDNegative = 1,
}ActionID;
/**
 * @}
 */

/**
 * @anchor AppAttributeIDs for Get App Attribute command
 * @name App Attribute ID values
 * @{
 */
typedef enum {
  AppAttributeIDDisplayName = 0,
} AppAttributeID;
/**
 * @}
 */


  

/**
 * @}
 */

/**
 * @}
 */

/**
 *@defgroup ANCS_NotifType ANCS Notification Attribute Types
 *@brief ANCS notification attributes types.
 *@{
 */

/**
 * @anchor notificationAttrS
 * @name Notification Attributes typedef
 * @{
 */
typedef struct notificationAttrS {
  /** A 32-bit numerical value that is the unique identifier (UID) for the iOS notification. This
   *  value can be used as a handle in commands sent to the Control Point characteristic to interact with the
   *  iOS notification.
   */
  uint32_t UID;
  /** App Identifier flag, if TRUE this attribute is requested from the Get Notification Attributes command */
  uint8_t  appID_flag;
  /** Title flag, if TRUE this attribute is requested from the Get Notification Attributes command */
  uint8_t  title_flag;
  /** Max title length parameter */
  uint16_t title_max_size;
  /** Subtitle flag, if TRUE this attribute is requested from the Get Notification Attributes command */
  uint8_t  subtitle_flag;
  /** Max Subtitle length parameter */
  uint16_t subtitle_max_size;
  /** Message flag, if TRUE this attribute is requested from the Get Notification Attributes command */
  uint8_t  message_flag;
  /** Max Message length parameter */
  uint16_t message_max_size;
  /** Message size flag, if TRUE this attribute is requested from the Get Notification Attributes command */
  uint8_t  messageSize_flag;
  /** Data flag, if TRUE this attribute is requested from the Get Notification Attributes command */
  uint8_t  date_flag;
  /** Positive Action flag, if TRUE this attribute is requested from the Get Notification Attributes command */
  uint8_t  positiveAction_flag;
  /** Negative Action flag, if TRUE this attribute is requested from the Get Notification Attributes command */
  uint8_t  negativeAction_flag;
} notificationAttr_type;
/**
 * @}
 */

/*-------------------------------------------------------------------------*/
  /**
   * Data Transfer structure (ie. Connection events state)
   */

  typedef enum
{
  ANCS_IDLE,
  ANCS_CONNECTED,             /* Connection_Evt: ANCS_CONNECTION_ESTABLISHED */
  ANCS_DISCONNECTING,      /* Connection_Evt */
  ANCS_DISCONN_COMPLETE,      /* Connection_Evt */
  
  ANCS_SET_DATA_LENGTH,       /* Connection_Evt */
  ANCS_MTU_UPDATE,            /* Connection_Evt */
  ANCS_MTU_EXCHANGE_COMPLETE, /* Connection_Evt */
  
  ANCS_DISCOVER_ALL_SERVICES,
  ANCS_DISCOVER_ANCS_SERVICE,
  ANCS_DISCOVER_ALL_CHARS,
  ANCS_DISCOVER_ALL_CHAR_DESCS,
  ANCS_ENABLE_NOTIFICATION_NOTIFICATION_SOURCE_DESC,
  ANCS_ENABLE_NOTIFICATION_DATA_SOURCE_DESC,
  ANCS_ENABLE_NOTIFICATION_SERVICE_CHANGED_DESC,
  
  ANCS_GET_NOTIFICATION_ATTRIBUTE,
  ANCS_GET_APP_ATTRIBUTE,
  ANCS_PERFORM_NOTIFICATION_ACTION,
  ANCS_CHECK_NOTIFICATION,
  
} ANCS_ProfileState; 

typedef struct
{
  /**
  * state of the Data Transfer (state machine)
  */
  
  ANCS_ProfileState Evt_Opcode;
  /**
  * connection handle
  */
  uint16_t connection_handle;
  
} Connection_Context_t;

/*-------------------------------------------------------------------------*/

  /* Includes ------------------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  
void ANCS_Client_App_Init(void);
void ANCS_App_KeyButton1Action(void);
void ANCS_App_KeyButton2Action(void);
void ANCS_App_KeyButton3Action(void);
void ANCS_App_Peer_Bonded_Check(uint16_t Connection_Handle, uint8_t Peer_Address_Type, uint8_t Peer_Address[6]);
void ANCS_App_Notification( Connection_Context_t *pNotification );

#ifdef __cplusplus
}
#endif

#endif /* ANCS_APPLICATION_H */
