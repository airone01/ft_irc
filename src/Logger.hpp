/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:42:42 by elagouch          #+#    #+#             */
/*   Updated: 2025/10/16 19:56:44 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Hi, this is a basic logger implementation to make our job easier later.
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

namespace logger {

enum LogLevel { DEBUG, INFO, WARNING, ERROR };

// the palette
// https://www.reddit.com/r/unixporn/comments/hjzw5f/oc_qualitative_color_palette_for_ansi_terminal/
namespace colors {
const char *const RESET = "\033[0m";
const char *const FG_BOLD_BG = "\033[1;30m";
const char *const BG_FG = "\033[47m";
// pastel
// https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124?permalink_comment_id=3892823#gistcomment-3892823
const char *const BG_ERR = "\033[48;2;239;166;162m";
const char *const BG_INFO = "\033[48;2;128;201;144m";
const char *const BG_WARN = "\033[48;2;200;200;116m";
const char *const BG_DEBUG = "\033[48;2;204;172;237m";
} // namespace colors

class Logger;

// this is to enable << operator chaining
class LogStream {
private:
  Logger &_logger;
  LogLevel _level;
  bool _first_output;

public:
  LogStream(Logger &logger, LogLevel level);
  ~LogStream();

  // template operator<< for any type
  template <typename T> LogStream &operator<<(const T &value);

  // special handling for manipulators (like std::endl)
  LogStream &operator<<(std::ostream &(*manip)(std::ostream &));

private:
  void writePrefix();
};

template <typename T> LogStream &LogStream::operator<<(const T &value) {
  if (_first_output) {
    writePrefix();
    _first_output = false;
  }
  std::cerr << value;
  return *this;
}

// (this is a singleton)
class Logger {
private:
  bool _colors_enabled;
  bool _show_timestamps;
  LogLevel _min_level;

  // private constructor bc singleton
  Logger();

  // prevent the compiler from implementing copying :-)
  Logger(const Logger &);
  Logger &operator=(const Logger &);

  bool detectColorSupport();

public:
  static Logger &getInstance();

  bool colorsEnabled() const;

  void setColorsEnabled(bool enabled);

  void setShowTimestamps(bool show);

  bool showTimestamps() const;

  void setMinLevel(LogLevel level);

  LogLevel getMinLevel() const;

  const char *getLevelColor(LogLevel level) const;

  const char *getLevelName(LogLevel level) const;

  const char *getReset() const;

  std::string getTimestamp() const;

  bool shouldLog(LogLevel level) const;
};

// Global log stream creators
LogStream debug();

LogStream info();

LogStream warning();

LogStream error();

} // namespace logger

#endif // LOGGER_H
