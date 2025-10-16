/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:42:42 by elagouch          #+#    #+#             */
/*   Updated: 2025/10/16 19:07:45 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#else
#include <unistd.h>
#endif

namespace logger {

LogStream::LogStream(Logger &logger, LogLevel level)
    : logger_(logger), level_(level), first_output_(true) {}

LogStream::~LogStream() {
  // this ensures we reset color at end if colors are enabled
  if (logger_.colorsEnabled()) {
    std::cerr << logger_.getReset();
  }
}

LogStream &LogStream::operator<<(std::ostream &(*manip)(std::ostream &)) {
  if (first_output_) {
    writePrefix();
    first_output_ = false;
  }
  manip(std::cerr);
  return *this;
}

void LogStream::writePrefix() {
  if (logger_.colorsEnabled()) {
    std::cerr << logger_.getLevelColor(level_) << colors::FG_BOLD_BG << " "
              << logger_.getLevelName(level_) << " " << logger_.getReset()
              << " ";
  } else {
    std::cerr << logger_.getTimestamp() << "[" << logger_.getLevelName(level_)
              << "] ";
  }
}

Logger::Logger() : show_timestamps_(true), min_level_(DEBUG) {
  colors_enabled_ = detectColorSupport();
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

bool Logger::colorsEnabled() const { return colors_enabled_; }

void Logger::setColorsEnabled(bool enabled) { colors_enabled_ = enabled; }

void Logger::setShowTimestamps(bool show) { show_timestamps_ = show; }

bool Logger::showTimestamps() const { return show_timestamps_; }

void Logger::setMinLevel(LogLevel level) { min_level_ = level; }

LogLevel Logger::getMinLevel() const { return min_level_; }

const char *Logger::getLevelColor(LogLevel level) const {
  if (!colors_enabled_) {
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

const char *Logger::getReset() const { return colors_enabled_ ? colors::RESET : ""; }

std::string Logger::getTimestamp() const {
  if (!show_timestamps_) {
    return "";
  }

  time_t now = time(NULL);
  struct tm *timeinfo = localtime(&now);
  char buffer[64];
  strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", timeinfo);
  return std::string(buffer);
}

bool Logger::shouldLog(LogLevel level) const { return level >= min_level_; }

LogStream debug() { return LogStream(Logger::getInstance(), DEBUG); }

LogStream info() { return LogStream(Logger::getInstance(), INFO); }

LogStream warning() { return LogStream(Logger::getInstance(), WARNING); }

LogStream error() { return LogStream(Logger::getInstance(), ERROR); }

} // namespace logger
