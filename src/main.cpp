/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:46:16 by elagouch          #+#    #+#             */
/*   Updated: 2025/10/16 18:28:03 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include <ostream>

int main() {
  // Basic usage with automatic color detection
  logger::info() << "Application started" << std::endl;
  logger::debug() << "Debug value: " << 42 << std::endl;
  logger::warning() << "This is a warning message" << std::endl;
  logger::error() << "Error code: " << -1 << std::endl;

  // Multiple values in one log line
  logger::info() << "Processing " << 100 << " items" << std::endl;

  // Disable colors manually if needed
  logger::Logger::getInstance().setColorsEnabled(false);
  logger::info() << "This will not have colors" << std::endl;

  // Re-enable colors
  logger::Logger::getInstance().setColorsEnabled(true);
  logger::info() << "Colors are back" << std::endl;

  // Disable timestamps
  logger::Logger::getInstance().setShowTimestamps(false);
  logger::info() << "No timestamp on this line" << std::endl;

  // Set minimum log level
  logger::Logger::getInstance().setMinLevel(logger::WARNING);
  logger::debug() << "This won't be shown" << std::endl;
  logger::info() << "This won't be shown either" << std::endl;
  logger::warning() << "But this will be shown" << std::endl;

  return 0;
}
