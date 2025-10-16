/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:42:42 by elagouch          #+#    #+#             */
/*   Updated: 2025/10/16 18:34:09 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Hi, this is a basic logger implementation to make our job easier later.
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#else
#include <unistd.h>
#endif

namespace logger {

enum LogLevel { DEBUG, INFO, WARNING, ERROR };

// the palette
// https://www.reddit.com/r/unixporn/comments/hjzw5f/oc_qualitative_color_palette_for_ansi_terminal/
namespace colors {
const char *const RESET       = "\033[0m";
const char *const FG_BOLD_BG  = "\033[1;30m";
const char *const BG_FG       = "\033[47m";
// pastel
// https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124?permalink_comment_id=3892823#gistcomment-3892823
const char *const BG_ERR      = "\033[48;2;239;166;162m";
const char *const BG_INFO     = "\033[48;2;128;201;144m";
const char *const BG_WARN     = "\033[48;2;200;200;116m";
const char *const BG_DEBUG    = "\033[48;2;204;172;237m";
} // namespace colors

class Logger;

// this is to enable << operator chaining
class LogStream {
private:
  Logger &logger_;
  LogLevel level_;
  bool first_output_;

public:
  LogStream(Logger &logger, LogLevel level);
  ~LogStream();

  // template operator<< for any type
  template <typename T> LogStream &operator<<(const T &value) {
    if (first_output_) {
      writePrefix();
      first_output_ = false;
    }
    std::cerr << value;
    return *this;
  }

  // special handling for manipulators (like std::endl)
  LogStream &operator<<(std::ostream &(*manip)(std::ostream &)) {
    if (first_output_) {
      writePrefix();
      first_output_ = false;
    }
    manip(std::cerr);
    return *this;
  }

private:
  void writePrefix();
};

// (this is a singleton)
class Logger {
private:
  bool colors_enabled_;
  bool show_timestamps_;
  LogLevel min_level_;

  // private constructor bc singleton
  Logger() : show_timestamps_(true), min_level_(DEBUG) {
    colors_enabled_ = detectColorSupport();
  }

  // prevent the compiler from implementing copying :-)
  Logger(const Logger &);
  Logger &operator=(const Logger &);

  bool detectColorSupport() {
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

public:
  static Logger &getInstance() {
    static Logger instance;
    return instance;
  }

  bool colorsEnabled() const { return colors_enabled_; }

  void setColorsEnabled(bool enabled) { colors_enabled_ = enabled; }

  void setShowTimestamps(bool show) { show_timestamps_ = show; }

  bool showTimestamps() const { return show_timestamps_; }

  void setMinLevel(LogLevel level) { min_level_ = level; }

  LogLevel getMinLevel() const { return min_level_; }

  const char *getLevelColor(LogLevel level) const {
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

  const char *getLevelName(LogLevel level) const {
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

  const char *getReset() const { return colors_enabled_ ? colors::RESET : ""; }

  std::string getTimestamp() const {
    if (!show_timestamps_) {
      return "";
    }

    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", timeinfo);
    return std::string(buffer);
  }

  bool shouldLog(LogLevel level) const { return level >= min_level_; }
};

inline LogStream::LogStream(Logger &logger, LogLevel level)
    : logger_(logger), level_(level), first_output_(true) {}

inline LogStream::~LogStream() {
  // this ensures we reset color at end if colors are enabled
  if (logger_.colorsEnabled()) {
    std::cerr << logger_.getReset();
  }
}

inline void LogStream::writePrefix() {
  if (logger_.colorsEnabled()) {
    std::cerr << logger_.getLevelColor(level_) << colors::FG_BOLD_BG << " "
              << logger_.getLevelName(level_) << " " << logger_.getReset()
              << " ";
  } else {
    std::cerr << logger_.getTimestamp() << "[" << logger_.getLevelName(level_)
              << "] ";
  }
}

// Global log stream creators
inline LogStream debug() { return LogStream(Logger::getInstance(), DEBUG); }

inline LogStream info() { return LogStream(Logger::getInstance(), INFO); }

inline LogStream warning() { return LogStream(Logger::getInstance(), WARNING); }

inline LogStream error() { return LogStream(Logger::getInstance(), ERROR); }

} // namespace logger

#endif // LOGGER_H
