
/*
 * Ekiga -- A VoIP and Video-Conferencing application
 * Copyright (C) 2000-2008 Damien Sandras

 * This program is free software; you can  redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version. This program is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Ekiga is licensed under the GPL license and as a special exception, you
 * have permission to link or otherwise combine this program with the
 * programs OPAL, OpenH323 and PWLIB, and distribute the combination, without
 * applying the requirements of the GNU GPL to the OPAL, OpenH323 and PWLIB
 * programs, as long as you do follow the requirements of the GNU GPL for all
 * the rest of the software thus combined.
 */


/*
 *                         audiooutput-core.cpp  -  description
 *                         ------------------------------------------
 *   begin                : written in 2008 by Matthias Schneider
 *   copyright            : (c) 2008 by Matthias Schneider
 *   description          : declaration of the interface of a audiooutput core.
 *                          A audiooutput core manages AudioOutputManagers.
 *
 */

#include "config.h"

#include "audiooutput-gmconf-bridge.h"
#include "audiooutput-core.h"

#define AUDIO_DEVICES_KEY "/apps/" PACKAGE_NAME "/devices/audio/"
#define AUDIO_CODECS_KEY "/apps/" PACKAGE_NAME "/codecs/audio/"
#define SOUND_EVENTS_KEY  "/apps/" PACKAGE_NAME "/general/sound_events/"

using namespace Ekiga;

AudioOutputCoreConfBridge::AudioOutputCoreConfBridge (Ekiga::Service & _service)
 : Ekiga::ConfBridge (_service)
{
  Ekiga::ConfKeys keys;
  property_changed.connect (sigc::mem_fun (this, &AudioOutputCoreConfBridge::on_property_changed));

  keys.push_back (AUDIO_DEVICES_KEY "output_device"); 
  keys.push_back (SOUND_EVENTS_KEY "output_device"); 
  keys.push_back (SOUND_EVENTS_KEY "busy_tone_sound"); 
  keys.push_back (SOUND_EVENTS_KEY "incoming_call_sound"); 
  keys.push_back (SOUND_EVENTS_KEY "new_message_sound"); 
  keys.push_back (SOUND_EVENTS_KEY "new_voicemail_sound"); 
  keys.push_back (SOUND_EVENTS_KEY "ring_tone_sound"); 
  keys.push_back (SOUND_EVENTS_KEY "enable_busy_tone_sound"); 
  keys.push_back (SOUND_EVENTS_KEY "enable_incoming_call_sound"); 
  keys.push_back (SOUND_EVENTS_KEY "enable_new_message_sound"); 
  keys.push_back (SOUND_EVENTS_KEY "enable_new_voicemail_sound"); 
  keys.push_back (SOUND_EVENTS_KEY "enable_ring_tone_sound"); 

  load (keys);
}

void AudioOutputCoreConfBridge::on_property_changed (std::string key, GmConfEntry *entry)
{
  std::string name;
  std::string file_name;
  bool enabled;

  AudioOutputCore & audioinput_core = (AudioOutputCore &) service;

  if (key == AUDIO_DEVICES_KEY "output_device") {

    PTRACE(4, "AudioOutputCoreConfBridge\tUpdating device");

    AudioOutputDevice audiooutput_device;

    if (gm_conf_entry_get_string (entry) == NULL) {
      PTRACE(1, "AudioOutputCoreConfBridge\t" << AUDIO_DEVICES_KEY "output_device" << " is NULL");
    }
    else {
      std::string config_string = gm_conf_entry_get_string (entry);
      unsigned type_sep = config_string.find_first_of("/");
      unsigned source_sep = config_string.find_first_of("/", type_sep + 1);

      audiooutput_device.type   = config_string.substr ( 0, type_sep );
      audiooutput_device.source = config_string.substr ( type_sep + 1, source_sep - type_sep - 1);
      audiooutput_device.device = config_string.substr ( source_sep + 1, config_string.size() - source_sep );  
    }
  
    if ( (audiooutput_device.type == "" )   ||
         (audiooutput_device.source == "")  ||
         (audiooutput_device.device == "" ) ) {
      PTRACE(1, "AudioOutputCore\tTried to set malformed device");
      audiooutput_device.type = AUDIO_OUTPUT_FALLBACK_DEVICE_TYPE;
      audiooutput_device.source = AUDIO_OUTPUT_FALLBACK_DEVICE_SOURCE;
      audiooutput_device.device = AUDIO_OUTPUT_FALLBACK_DEVICE_DEVICE;
    }

    audioinput_core.set_audiooutput_device (primary, audiooutput_device);
  }

  if (key == SOUND_EVENTS_KEY "output_device") {

    PTRACE(4, "AudioOutputCoreConfBridge\tUpdating device");
    AudioOutputDevice audiooutput_device;

    if (gm_conf_entry_get_string (entry) == NULL) {
      PTRACE(1, "AudioOutputCoreConfBridge\t" << AUDIO_DEVICES_KEY "output_device" << " is NULL");
    }
    else {
      std::string config_string = gm_conf_entry_get_string (entry);

      unsigned type_sep = config_string.find_first_of("/");
      unsigned source_sep = config_string.find_first_of("/", type_sep + 1);

      audiooutput_device.type   = config_string.substr ( 0, type_sep );
      audiooutput_device.source = config_string.substr ( type_sep + 1, source_sep - type_sep - 1);
      audiooutput_device.device = config_string.substr ( source_sep + 1, config_string.size() - source_sep );
    }

    if ( (audiooutput_device.type == "" )   ||
         (audiooutput_device.source == "")  ||
         (audiooutput_device.device == "" ) ) {
      PTRACE(1, "AudioOutputCore\tTried to set malformed device");
      audiooutput_device.type = AUDIO_OUTPUT_FALLBACK_DEVICE_TYPE;
      audiooutput_device.source = AUDIO_OUTPUT_FALLBACK_DEVICE_SOURCE;
      audiooutput_device.device = AUDIO_OUTPUT_FALLBACK_DEVICE_DEVICE;
    }
    audioinput_core.set_audiooutput_device (secondary, audiooutput_device);
  }

  if ( (key == SOUND_EVENTS_KEY "busy_tone_sound") ||
       (key == SOUND_EVENTS_KEY "enable_busy_tone_sound") ) {

    if (gm_conf_get_string (SOUND_EVENTS_KEY "busy_tone_sound") == NULL) {
      PTRACE(1, "AudioOutputCoreConfBridge\t" << SOUND_EVENTS_KEY "busy_tone_sound" << " is NULL");
      return;
    }

    name = "busy_tone_sound";
    file_name = gm_conf_get_string (SOUND_EVENTS_KEY "busy_tone_sound");
    enabled = gm_conf_get_bool (SOUND_EVENTS_KEY "enable_busy_tone_sound");
    audioinput_core.add_event (name, file_name, enabled, primary);
  }

  if ( (key == SOUND_EVENTS_KEY "incoming_call_sound") ||
       (key == SOUND_EVENTS_KEY "enable_incoming_call_sound") ) {

    if (gm_conf_get_string (SOUND_EVENTS_KEY "incoming_call_sound") == NULL) {
      PTRACE(1, "AudioOutputCoreConfBridge\t" <<  SOUND_EVENTS_KEY "incoming_call_sound" << " is NULL");
      return;
    }

    name = "incoming_call_sound";
    file_name = gm_conf_get_string (SOUND_EVENTS_KEY "incoming_call_sound");
    enabled = gm_conf_get_bool (SOUND_EVENTS_KEY "enable_incoming_call_sound");
    audioinput_core.add_event (name, file_name, enabled, secondary);
  }

  if ( (key == SOUND_EVENTS_KEY "new_message_sound") ||
       (key == SOUND_EVENTS_KEY "enable_new_message_sound") ) {

    if (gm_conf_get_string (SOUND_EVENTS_KEY "new_message_sound") == NULL) {
      PTRACE(1, "AudioOutputCoreConfBridge\t" <<  SOUND_EVENTS_KEY "new_message_sound" << " is NULL");
      return;
    }

    name = "new_message_sound";
    file_name = gm_conf_get_string (SOUND_EVENTS_KEY "new_message_sound");
    enabled = gm_conf_get_bool (SOUND_EVENTS_KEY "enable_new_message_sound");
    audioinput_core.add_event (name, file_name, enabled, secondary);

  }

  if ( (key == SOUND_EVENTS_KEY "new_voicemail_sound") ||
       (key == SOUND_EVENTS_KEY "enable_new_voicemail_sound") ) {

    if (gm_conf_get_string (SOUND_EVENTS_KEY "new_voicemail_sound") == NULL) {
      PTRACE(1, "AudioOutputCoreConfBridge\t" <<  SOUND_EVENTS_KEY "new_voicemail_sound" << " is NULL");
      return;
    }

    name = "new_voicemail_sound";
    file_name = gm_conf_get_string (SOUND_EVENTS_KEY "new_voicemail_sound");
    enabled = gm_conf_get_bool (SOUND_EVENTS_KEY "enable_new_voicemail_sound");
    audioinput_core.add_event (name, file_name, enabled, secondary);
  }

  if ( (key == SOUND_EVENTS_KEY "ring_tone_sound") ||
       (key == SOUND_EVENTS_KEY "enable_ring_tone_sound") ) {

    if (gm_conf_get_string (SOUND_EVENTS_KEY "ring_tone_sound") == NULL) {
      PTRACE(1, "AudioOutputCoreConfBridge\t" <<  SOUND_EVENTS_KEY "ring_tone_sound" << " is NULL");
      return;
    }

    name = "ring_tone_sound";
    file_name = gm_conf_get_string (SOUND_EVENTS_KEY "ring_tone_sound");
    enabled = gm_conf_get_bool (SOUND_EVENTS_KEY "enable_ring_tone_sound");
    audioinput_core.add_event (name, file_name, enabled, primary);
  }
}
