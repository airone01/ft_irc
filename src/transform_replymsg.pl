#!/usr/bin/env perl
use strict;
use warnings;

my $file = shift or die "Usage: $0 <input-file>\n";

# Read entire file manually (no modules required)
my $text = "";
{
    local $/ = undef;
    open my $fh, "<", $file or die "Cannot open '$file': $!";
    $text = <$fh>;
    close $fh;
}

# Ensure includes
unless ($text =~ /#\s*include\s*<sstream>/) {
    $text =~ s/(#\s*include\s*".*?ReplyMessage\.hpp".*\n)/$1#include <sstream>\n/
        or $text =~ s/(#\s*include\s*<[^>]+>\s*\n)/$1#include <sstream>\n/
        or $text = "#include <sstream>\n$text";
}

unless ($text =~ /#\s*include\s*<string>/) {
    $text =~ s/(#\s*include\s*<sstream>\s*\n)/$1#include <string>\n/
        or $text = "#include <string>\n$text";
}

# Process functions
my $out = "";
pos($text) = 0;

while (
    $text =~
    /\G(.*?)(\b(?:void|std::string)\s+ReplyMessage::[A-Za-z_]\w*\s*\([^)]*\)\s*\{)/sgc
) {
    $out .= $1;

    my $sig_start = pos($text) - length($2);
    my $signature = $2;

    # Find function body braces
    my $start = pos($text) - 1;
    my $depth = 0;
    my $found_open = 0;
    my $len = length($text);
    my $i = $start;

    for (; $i < $len; $i++) {
        my $c = substr($text, $i, 1);
        if ($c eq '{') {
            $depth++; $found_open = 1;
        } elsif ($c eq '}') {
            $depth--;
            last if $found_open && $depth == 0;
        }
    }

    my $func = substr($text, $sig_start, $i - $sig_start + 1);

    # Split into signature/body
    unless ($func =~ /^(\b(?:void|std::string)\s+ReplyMessage::[A-Za-z_]\w*\s*\([^)]*\)\s*)\{(.*)\}\s*$/s) {
        $out .= $func;
        pos($text) = $i + 1;
        next;
    }

    my ($sig, $body) = ($1, $2);

    # Force return type to std::string
    $sig =~ s/^\s*(?:void|std::string)/std::string/;

    # Find std::cout statements
    my @couts = ($body =~ /(std::cout\s*<<.*?;)/sg);
    my $new_body = "";

    if (@couts) {
        $new_body .= "\n    std::ostringstream oss;\n";

        foreach my $stmt (@couts) {
            $stmt =~ s/;\s*$//;
            $stmt =~ s/<<\s*std::endl\b//g;
            $stmt =~ s/^\s*std::cout/oss/;
            $stmt =~ s/\n/\n    /g;
            $new_body .= "    $stmt;\n";
        }

        $new_body .= "    std::string v = oss.str();\n";
        $new_body .= "    logger::error() << v;\n";
        $new_body .= "    return v;\n";

    } else {
        my $trim = $body;
        $trim =~ s/^\s+|\s+$//g;

        if ($trim eq "") {
            $new_body .= "\n    std::string v;\n    logger::error() << v;\n    return v;\n";
        } else {
            $trim =~ s/^/    /mg;
            $new_body .= "\n$trim\n";
            $new_body .= "    std::ostringstream oss;\n";
            $new_body .= "    std::string v = oss.str();\n";
            $new_body .= "    logger::error() << v;\n";
            $new_body .= "    return v;\n";
        }
    }

    # Rebuild function
    $out .= $sig . "{";
    $out .= $new_body;
    $out .= "}\n";

    pos($text) = $i + 1;
}

$out .= substr($text, pos($text)) if pos($text) < length($text);

print $out;
