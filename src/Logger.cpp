/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:42:42 by elagouch          #+#    #+#             */
/*   Updated: 2025/10/16 20:23:07 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

#include <cstdlib>
#include <ctime>
#include <unistd.h>

namespace logger {

LogStream::LogStream(Logger &logger, LogLevel level)
    : _logger(logger), _level(level), _first_output(true) {}

LogStream::~LogStream() {
  // this ensures we reset color at end if colors are enabled
  if (_logger.colorsEnabled()) {
    std::cerr << _logger.getReset();
  }
}

LogStream &LogStream::operator<<(std::ostream &(*manip)(std::ostream &)) {
  if (!_logger.shouldLog(_level)) {
    return *this;
  }
  if (_first_output) {
    writePrefix();
    _first_output = false;
  }
  manip(std::cerr);
  return *this;
}

void LogStream::writePrefix() {
  if (_logger.colorsEnabled()) {
    std::cerr << _logger.getLevelColor(_level) << colors::FG_BOLD_BG << " "
              << _logger.getLevelName(_level) << " " << _logger.getReset()
              << " ";
  } else {
    std::cerr << _logger.getTimestamp() << "[" << _logger.getLevelName(_level)
              << "] ";
  }
}

Logger::Logger() : _show_timestamps(true), _min_level(INFO) {
  _colors_enabled = detectColorSupport();
}

bool Logger::detectColorSupport() {
  if (!isatty(STDERR_FILENO)) {
    return false;
  }

  // https://no-color.org/
  if (std::getenv("NO_COLOR") != NULL) {
    return false;
  }

  const char *term = std::getenv("TERM");
  if (term == NULL) {
    return false;
  }

  std::string term_str(term);

  if (term_str == "dumb" || term_str == "unknown") {
    return false;
  }

  // common modern color terminal types
  if (term_str.find("color") != std::string::npos ||
      term_str.find("xterm") != std::string::npos ||
      term_str.find("screen") != std::string::npos ||
      term_str.find("linux") != std::string::npos ||
      term_str.find("cygwin") != std::string::npos ||
      term_str.find("rxvt") != std::string::npos) {
    return true;
  }

  // default for no color if not matching
  return false;
}

Logger &Logger::getInstance() {
  static Logger instance;
  return instance;
}

bool Logger::colorsEnabled() const { return _colors_enabled; }

void Logger::setColorsEnabled(bool enabled) { _colors_enabled = enabled; }

void Logger::setShowTimestamps(bool show) { _show_timestamps = show; }

bool Logger::showTimestamps() const { return _show_timestamps; }

void Logger::setMinLevel(LogLevel level) { _min_level = level; }

LogLevel Logger::getMinLevel() const { return _min_level; }

const char *Logger::getLevelColor(LogLevel level) const {
  if (!_colors_enabled) {
    return "";
  }

  switch (level) {
  case DEBUG:
    return colors::BG_DEBUG;
  case INFO:
    return colors::BG_INFO;
  case WARNING:
    return colors::BG_WARN;
  case ERROR:
    return colors::BG_ERR;
  default:
    return colors::BG_FG;
  }
}

const char *Logger::getLevelName(LogLevel level) const {
  switch (level) {
  case DEBUG:
    return "DBG";
  case INFO:
    return "NFO";
  case WARNING:
    return "WRN";
  case ERROR:
    return "ERR";
  default:
    return "IDK";
  }
}

const char *Logger::getReset() const {
  return _colors_enabled ? colors::RESET : "";
}

std::string Logger::getTimestamp() const {
  if (!_show_timestamps) {
    return "";
  }

  time_t now = time(NULL);
  struct tm *timeinfo = localtime(&now);
  char buffer[64];
  strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", timeinfo);
  return std::string(buffer);
}

bool Logger::shouldLog(LogLevel level) const { return level >= _min_level; }

LogStream debug() { return LogStream(Logger::getInstance(), DEBUG); }

LogStream info() { return LogStream(Logger::getInstance(), INFO); }

LogStream warning() { return LogStream(Logger::getInstance(), WARNING); }

LogStream error() { return LogStream(Logger::getInstance(), ERROR); }

} // namespace logger
