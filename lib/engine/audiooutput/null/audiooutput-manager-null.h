
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
 *                         audioinput-manager-null.h  -  description
 *                         ------------------------------------------
 *   begin                : written in 2008 by Matthias Schneider
 *   copyright            : (c) 2008 by Matthias Schneider
 *   description          : declaration of the interface of a audioinput core.
 *                          A audioinput core manages AudioInputManagers.
 *
 */


#ifndef __AUDIOOUTPUT_MANAGER_NULL_H__
#define __AUDIOOUTPUT_MANAGER_NULL_H__

#include "audiooutput-manager.h"
#include "runtime.h"

#include "ptbuildopts.h"
#include <ptclib/delaychan.h>

/**
 * @addtogroup audioinput
 * @{
 */

  class GMAudioOutputManager_null
   : public Ekiga::AudioOutputManager
    {
  public:

      /* The constructor
       */
       GMAudioOutputManager_null (Ekiga::ServiceCore & core);
      /* The destructor
       */
      ~GMAudioOutputManager_null () {}


      /*                 
       * DISPLAY MANAGEMENT 
       */               

      /** Create a call based on the remote uri given as parameter
       * @param uri  an uri
       * @return     true if a Ekiga::Call could be created
       */

      virtual void get_audiooutput_devices (std::vector <Ekiga::AudioOutputDevice> & audiooutput_devices);

      virtual bool set_audiooutput_device (Ekiga::AudioOutputPrimarySecondary primarySecondary, const Ekiga::AudioOutputDevice & audiooutput_device);

      virtual bool open (Ekiga::AudioOutputPrimarySecondary primarySecondary, unsigned channels, unsigned samplerate, unsigned bits_per_sample);

      virtual void close(Ekiga::AudioOutputPrimarySecondary primarySecondary);

      virtual bool set_frame_data (Ekiga::AudioOutputPrimarySecondary primarySecondary, 
                                   char *data, 
                                   unsigned size,
				   unsigned & written);
      virtual bool has_device     (const std::string & sink, const std::string & device, Ekiga::AudioOutputDevice & audiooutput_device);

    protected:
      Ekiga::ServiceCore & core;
      Ekiga::Runtime & runtime;

      PAdaptiveDelay m_Pacing[2];

  };
/**
 * @}
 */


#endif