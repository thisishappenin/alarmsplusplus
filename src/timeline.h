//
//  timeline.h
//  alarmsplusplus
//
//  Created by Christian Reinbacher on 24.11.15.
//
//

#ifndef timeline_h
#define timeline_h

#include <alarms.h>
void setup_communication(void);
void alarm_phone_send_pin(Alarm* alarm);
void alarm_phone_delete_pin(void);
#endif /* timeline_h */
