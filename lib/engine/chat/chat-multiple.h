
/*
 * Ekiga -- A VoIP and Video-Conferencing application
 * Copyright (C) 2000-2009 Damien Sandras <dsandras@seconix.com>

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
 *                         chat-multiple.h  -  description
 *                         ------------------------------------------
 *   begin                : written in 2007 by Julien Puydt
 *   copyright            : (c) 2007 by Julien Puydt
 *   description          : declaration of the interface of a chat with a
 *                          several persons
 *
 */

#ifndef __CHAT_MULTIPLE_H__
#define __CHAT_MULTIPLE_H__

#include <boost/smart_ptr.hpp>

#include "chat.h"
#include "heap.h"

namespace Ekiga
{

  class MultipleChat: public Chat
  {
  public:

    /** The destructor.
     */
    virtual ~MultipleChat ()
    {}

    /** Returns the Heap associated with the MultipleChat.
     * @return The MultipleChat's Heap
     */
    virtual HeapPtr get_heap () const = 0;
  };

  typedef boost::shared_ptr<MultipleChat> MultipleChatPtr;

};

#endif
