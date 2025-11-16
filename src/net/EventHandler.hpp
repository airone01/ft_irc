/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:42:35 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/12 16:00:36 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_CPP
#define EVENTHANDLER_CPP

#include <stdint.h>

/**
 * @brief Minimal abstract base for things registered with the Reactor (epoll).
 *
 * @note This will never be implemented, only used as a parent, and thus won't
 *       have a dedicated .cpp implementation file.
 */
class EventHandler {
public:
  virtual ~EventHandler() {}

  /**
   * @brief Called by Reactor when the associated fd is ready.
   * @param events Bitmask of epoll events (EPOLLIN/EPOLLOUT/EPOLLERR/...).
   */
  virtual void handleEvent(uint32_t events) = 0;
};

#endif // !EVENTHANDLER_CPP
