#!/usr/bin/tclsh
proc oblet_class {cls_nam args} {
   # Oblet class names can contain dots, but no other funny characters.
   if { [regexp {[^a-zA-Z0-9_\.]} $cls_nam] } {
      panic "Cannot create oblet class; '$cls_nam' contains funny characters."
   }

   if { [info commands $cls_nam] != "" } {
      panic "Cannot create oblet class; '$cls_nam' already exists."
   }

   #######
   # Class members.

   # Fill an array with default values for member variables.
   upvar #0 oblet_v_$cls_nam cls_arr
   set cls_arr(name) $cls_nam

   # Inherit attributes and methods from base classes.
   if { [lindex $args 0] == ":" } {
      foreach base [lrange $args 1 [expr [llength $args] - 1]] {
         # Attributes are fetched from the base class array,
         # so you also inherit new attributes injected after the base class was declared.
         upvar #0 oblet_v_$base base_arr
         foreach a [array names base_arr "-*"] {
            # Only copy attrs with a leading dash, to avoid copying special attrs like 'class'.
            set cls_arr($a) $base_arr($a)
         }

         # Methods are fetched using 'info procs',
         # so you also inherit new methods defined after the base class was declared.
         # You can always override the default body we install here.
         foreach m [info procs $base:*] {
            regsub {^[^:]+:} $m "" m2
            proc ${cls_nam}:${m2} {args} "return \[uplevel ${m} \$args\]"
         }
      }
   }

   # Define new local member variables using 'set',
   # and new member functions using 'proc', possibly overriding base class functions.
   rename proc oblet_tmp_proc
   rename set  oblet_tmp_set

   oblet_tmp_proc set {var_nam val} {
      upvar cls_arr cls_arr
      oblet_tmp_set cls_arr(-$var_nam) $val
   }

   oblet_tmp_proc proc {proc_nam arglist body} {
      upvar cls_nam cls_nam
      oblet_tmp_proc ${cls_nam}:${proc_nam} [linsert $arglist 0 this] $body
   }

   # Evaluate the class body which contains 'proc' and 'set' commands.
   eval [lindex $args end]

   rename proc ""
   rename set ""

   rename oblet_tmp_proc proc
   rename oblet_tmp_set set

   #######
   # Create the instance array and instance command.

   # To avoid backslash hell, we use the '@' trick.
   set ctor_template {
      if { [regexp {[^a-zA-Z0-9_\.\-]} $obj_nam] } {
         panic "Cannot create oblet; '$obj_nam' contains funny characters."
      }
      if { [info commands $obj_nam] != "" } {
         panic "Cannot create oblet; '$obj_nam' already exists."
      }
      upvar #0 oblet_v_@cls_nam@ cls_arr
      upvar #0 oblet_v_$obj_nam  obj_arr

      set obj_arr(class) @cls_nam@

      # Copy members from class.
      foreach a [array names cls_arr "-*"] {
         set obj_arr($a) $cls_arr($a)
      }

      # Override defaults from ctor args.
      foreach {a v} $args {
         set obj_arr($a) $v
      }

      ###
      # Instance command

      set obj_template {
         if { [info procs @obj_nam@:$cmd] != "" } {
            # Instance-specific method.
            return [uplevel @obj_nam@:$cmd @obj_nam@ $args]
         } else {
            # Class-wide method.
            upvar #0 oblet_v_@obj_nam@ obj_arr
            return [uplevel $obj_arr(class):$cmd @obj_nam@ $args]
         }
      }

      regsub -all {@obj_nam@} $obj_template "$obj_nam" obj_template
      proc ${obj_nam} {cmd args} $obj_template

      # Call constructor if it exists.
      if { [info procs "@cls_nam@:ctor"] != "" } {
         uplevel "@cls_nam@:ctor" $obj_nam
      }
   }

   regsub -all {@cls_nam@} $ctor_template "$cls_nam" ctor_template
   proc ${cls_nam} {obj_nam args} $ctor_template

   #######
   # Some standard methods.

   # Give new values to one or more attrs.
   proc ${cls_nam}:set {this args} {
      upvar #0 oblet_v_$this obj_arr

      # Note that class-static attrs are not supported.
      # All attrs go into the obj-specific array.
      foreach {a v} $args {
         set obj_arr($a) $v
      }
   }

   # Get the value of an attr.
   proc ${cls_nam}:get {this a} {
      upvar #0 oblet_v_$this obj_arr
      return $obj_arr($a)
   }

   # Get a number of attr values into local variables with the same name.
   proc ${cls_nam}:members {this args} {
      upvar #0 oblet_v_$this obj_arr
      foreach arg $args {
         upvar $arg my_arg
            # We have to go only 1 level up: the instance command does 'uplevel'!
         set my_arg $obj_arr(-$arg)
      }
   }
}


# Delete one or more oblets.
# If they don't exist, ignore (no error).
proc oblet_delete {args} {
   foreach obj_nam $args {
      upvar #0 oblet_v_$obj_nam obj_arr
      if { [info exists obj_arr] } {
         set cls_nam $obj_arr(class)

         # Call the destructor before deleting any of the members.
         if { [info procs "${obj_nam}:dtor"] != "" } {
            # Instance-specific destructor.
            uplevel "${obj_nam}:dtor" ${obj_nam}
         } elseif { [info procs "${cls_nam}:dtor"] != "" } {
            # Class destructor.
            uplevel "${cls_nam}:dtor" ${obj_nam}
         }

         unset obj_arr
            # Delete the attributes array.
         foreach p [info procs "${obj_nam}:"] {
            # Delete all instance-specific methods.
            rename $p {}
         }
         rename $obj_nam {}
            # Delete the instance command itself.
      }
   }
}

oblet_class Line {
   # Represents one line of input.

   set lang_obj ""
      # The language object that claimed this line (either the prepro or target language).
   set indent ""
      # The indentation part of the line (i.e. the leading spaces).
   set text ""
      # The rest of the line after initial indentation.
   set marker ""
      # The marker with which this line is marked.

   proc mark {} {
      # Places the first character of 'text' in 'marker',
      # replaces it with a space, and adapts the indentation.

      $this members indent text

      if { $text == "" } { return }

      regexp {^(.)( *)(.*)$} $text -> marker new_indent new_text
      append indent " $new_indent"
         # The single extra space replaces the marker itself.

      $this set -indent $indent -text $new_text -marker $marker
   }
}

oblet_class Language {
   # One of the language objects that claims lines in the input file.
   # Each line should be claimed by exactly one language object.

   set name ""
      # Name of this language object, e.g. 'tcl' or 'cxx'.
   set main ""
      # The object of class LineProcessor that controls the preprocessing algorithm.
   set marker ""
      # The marker with which lines of this language are marked.

   proc init_file {fod} {
      # Write a header to the output file.  Empty by default.
   }

   proc exit_file {fod} {
      # Write a footer to the output file.  Empty by default.
   }

   proc output {fod prepro_indent text} {
      # Output text verbatim to the intermediate script.
      puts $fod "${prepro_indent}${text}"
   }
}

oblet_class Language_txt : Language {
   # Dummy class for outputting any kind of text.
}

oblet_class Language_cxx : Language {
   # Dummy class for outputting C++ code.
}

oblet_class Language_tcl : Language {
   proc init_file {fod} {
      puts $fod "#!/usr/bin/tclsh"
      puts $fod ""
   }

   proc decorate {fod prepro_indent target_indent text} {
      # Decorate a line and send it to the intermediate script.

      # First, escape all special characters and expand the '$(...)' substitutions.
      set str ""
      for {set i 0} {$i < [string length $text]} {incr i} {
         set c [string index $text $i]

         if { $c == "\$" } {
            incr i
            set c [string index $text $i]
            if { $c != "(" } {
               append str "\\\$"
               incr i -1
               continue
            }

            append str "\$\{"
            incr i
            set c [string index $text $i]
            while { $c != ")" } {
               append str $c
               incr i
               set c [string index $text $i]
            }
            append str "\}"

            continue
         }

         if {    $c == "\\" || $c == "\$" \
              || $c == "\{" || $c == "\}" \
              || $c == "\[" || $c == "\]" \
              || $c == "\"" } {
            append str "\\"
         }
         append str $c
      }

      puts $fod "${prepro_indent}puts \"${target_indent}${str}\""
   }

   proc execute {script output_filename} {
      # Execute 'script' and send its output to 'output_filename'.
      catch {eval exec /usr/bin/tclsh $script > $output_filename} result
   }
}

oblet_class Language_lua : Language {
   proc decorate {fod prepro_indent target_indent text} {
      # Decorate a line and send it to the intermediate script.

      set str ""
      for {set i 0} {$i < [string length $text]} {incr i} {
         set c [string index $text $i]
         if {    $c == "\\" || $c == "\%" \
              || $c == "\"" || $c == "\'" } {
            append str "\\"
         }
         append str $c
      }

      # String substitution in Lua is done with the '..' concatenation operator.
      while { [regexp {^(.*)\$\(([a-zA-Z0-9_]+)\)(.*)$} $str -> pre var post] } {
         set str "${pre}\" .. ${var} .. \"${post}"
      }

      puts $fod "${prepro_indent}print(\"${target_indent}${str}\")"
   }

   proc execute {script output_filename} {
      # Execute 'script' and send its output to 'output_filename'.
      catch {eval exec /usr/bin/lua $script} result
      set fod [open $output_filename w]
      puts $fod $result
      close $fod
   }
}

oblet_class Language_ruby : Language {
   proc decorate {fod prepro_indent target_indent text} {
      # Decorate a line and send it to the intermediate script.

      set str ""
      for {set i 0} {$i < [string length $text]} {incr i} {
         set c [string index $text $i]
         if {    $c == "\\" || $c == "\#" \
              || $c == "\"" || $c == "\'" } {
            append str "\\"
         }
         append str $c
      }

      # String substitution in Ruby is done with '#'.
      while { [regexp {^(.*)\$\(([a-zA-Z0-9_]+)\)(.*)$} $str -> pre var post] } {
         set str "${pre}#\{${var}\}${post}"
      }

      puts $fod "${prepro_indent}puts \"${target_indent}${str}\""
   }

   proc execute {script output_filename} {
      # Execute 'script' and send its output to 'output_filename'.
      catch {eval exec /usr/bin/ruby $script} result
      set fod [open $output_filename w]
      puts $fod $result
      close $fod
   }
}

oblet_class Frame {
   # Stack frames that represent the current input blocks.
   # Every indented block in the input creates one of these frames on the main stack.
   # All member variables below are local to the current block;
   # when the block gets popped, the previous values become active again.

   set indent ""
      # The indentation of the first line line of this block.
      # All consecutive lines in the block must have exactly this indentation.
   set indent_prepro ""
      # Part of the indentation that belongs to the prepro-language.
   set indent_target ""
      # Part of the indentation that belongs to the target language.

   # Invariant:  indent == indent_prepro + indent_target.
   # - The defaults above establish the invariant.
   # - When we push a new frame, we take the extra indentation FROM 'indent',
   #   and append it to either indent_prepro OR indent_target.

   set cur_lang_obj ""
      # Default language object (for unmarked lines).
}

oblet_class LineProcessor {
   # The main object that performs the preprocessing algorithm.

   set lines [list]
      # List of all line objects (instances of Line).
   set prev_non_empty_line ""
      # Previous non-blank line (instance of class Line, one of the elements in 'lines').

   set stack [list]
      # Stack of frames (instances of Frame),
      # one for each nested block in the input.

   ### Read a line from the input file, and split it into indentation and text.
   proc read_line {fid} {
      if { [eof $fid] } {
         return ""
      }

      gets $fid line
      regexp {^( *)(.*)$} $line -> indent rest

      $this members lines
      set result "line_[llength $lines]"
      Line $result -indent $indent -text $rest

      lappend lines $result
      $this set -lines $lines

      return $result
   }

   ### Create new language objects.
   proc declare_lang {obj_name lang_name lang_args} {
      # Set the default attributes for this language object.
      Language_$lang_name $obj_name \
         -main $this \
         -name $lang_name \
         -marker ""
      if {![regexp {^ *$} $lang_args]} {
         # Set other attributes from the declaration in the input.
         eval $obj_name set $lang_args
      }

      # This language becomes the default one if there was no default yet.
      $this members stack
      set top_frame [lindex $stack end]
      if { [$top_frame get -cur_lang_obj] == "" } {
         $top_frame set -cur_lang_obj $obj_name
      }
   }

   ### Find language objects by name.
   proc find_lang_obj {name} {
      if { [prepro_lang_obj get -name] == $name } { return "prepro_lang_obj" }
      if { [target_lang_obj get -name] == $name } { return "target_lang_obj" }
      return ""
   }

   ### Claim a line based on its explicit marker, if any.
   proc claim_line {cur_line} {
      $cur_line members text

      # Ask each language object if it recognizes the line's marker.
      foreach lang_obj [list directive_lang_obj prepro_lang_obj target_lang_obj] {
         set marker [$lang_obj get -marker]
         if { ($marker != "") && ([string index $text 0] == $marker) } {
            if { $lang_obj != "directive_lang_obj" } {
               $cur_line mark
            }
            return $lang_obj
         }
      }

      return ""  ; # Nobody recognized the marker.
   }

   ### Push a new frame onto the stack.
   proc push_frame {} {
      $this members stack
      set frame_obj "frame_[llength $stack]"
      Frame $frame_obj

      if { [llength $stack] > 0 } {
         # Inherit current language from parent frame until further notice.
         set parent_frame [lindex $stack end]
         $frame_obj set -cur_lang_obj [$parent_frame get -cur_lang_obj]
      }
      lappend stack $frame_obj
      $this set -stack $stack

      return $frame_obj
   }

   ### Push/pop frames based on the indentation in the input line.
   proc handle_indent {cur_line} {
      $cur_line members indent text

      $this members stack
      set top_frame [lindex $stack end]

      if { [regexp {^ *$} $text] } {
         # Ignore empty lines.  They're always part of the current block.
         # Note that empty lines never have an indentation, so they must not influence
         # our indentation mechanism.
         return
      }

      set did_backtrack false

      while { [string length [$top_frame get -indent]] > [string length $indent] } {
         # Back out of a block.
         set did_backtrack true
         oblet_delete $top_frame
         set stack [lreplace $stack end end]
         set top_frame [lindex $stack end]
      }
      if { $did_backtrack != "false" } {
         $this set -stack $stack
      }

      set cur_indent [$top_frame get -indent]
      if { [string length $cur_indent] < [string length $indent] } {
         # Go into new block.  The indentation jumps by a new amount.

         if { $did_backtrack != "false" } {
            error "Wrong indentation."
         }

         $top_frame members indent_prepro indent_target
         regexp "^${cur_indent}( *)\$" $indent -> new_ind

         # Determine new indentations based on who caused the jump.
         $this members prev_non_empty_line
         if { $prev_non_empty_line == "" } {
            error "You cannot indent without a leading line."
         }
         if { [$prev_non_empty_line get -lang_obj] == "prepro_lang_obj" } {
            # The indentation is in the prepro language.
            append indent_prepro $new_ind
         } else {
            # The indentation is in the target language.
            append indent_target $new_ind
         }

         # Push new block with same default lang as parent block.
         set top_frame [$this push_frame]

         # Give the new block its indentation values.
         $top_frame set \
            -indent $indent \
            -indent_prepro $indent_prepro \
            -indent_target $indent_target
      }

      # In all other cases, stay in current block.
   }

   ### Produce the intermediate script.
   proc produce_script {infile outfile} {
      set fid [open $infile r]
      set fod [open $outfile w]

      # Read the first line.  It gives us the directive-marker.
      set line [$this read_line $fid]
      set directive_char [string index [$line get -text] 0]

      # Instantiate a dummy language object for the directive language.
      Language directive_lang_obj -name "directive" -main $this -marker $directive_char

      # Instantiate the preprocessor language from the first input line.
      regexp {^.\! +([^ ]+) *(.*)$} [$line get -text] -> lang_name lang_args
      $this declare_lang prepro_lang_obj $lang_name $lang_args

      # Instantiate the target language from the second input line.
      set line [$this read_line $fid]
      regexp {^.\! +([^ ]+) *(.*)$} [$line get -text] -> lang_name lang_args
      $this declare_lang target_lang_obj $lang_name $lang_args

      # Write a header to the output file.
      prepro_lang_obj init_file $fod

      # Go over the input and process it line by line.
      while { true } {
         set cur_line [$this read_line $fid]
         if { $cur_line == "" } {
            # Reached end of input file.  Done.
            break
         }

         $cur_line members indent text

         $this members stack
         set top_frame [lindex $stack end]
         $top_frame members cur_lang_obj

         ###
         # Determine the line's language.
         # We need to do this *first*, because marker-based recognition
         # may alter the line's indentation.

         set recognized_lang_obj [$this claim_line $cur_line]
         if { $recognized_lang_obj != ""} {
            # After marking, both the indent and the text may have changed.
            $cur_line members indent text
         }

         ###
         # Update the stack based on the current line's indentation.

         $this handle_indent $cur_line

         # The stack may have changed.  In fact, the current (default) language may also have changed,
         # which is why we can continue the recognition process only *after* this.
         $this members stack
         set top_frame [lindex $stack end]
         $top_frame members cur_lang_obj

         ###
         # As a last resort, use the current default language.

         if { $recognized_lang_obj == "" } {
            # If not set by a marker, choose a good language for the line.
            # The best choice is simply the currently active language, the one
            # on the top of the stack.
            set recognized_lang_obj $cur_lang_obj
         }

         # The line's language should be determined now.
         $cur_line set -lang_obj $recognized_lang_obj

         ###
         # Parse the input line & process it if needed.

         $top_frame members indent_prepro indent_target

         if { $recognized_lang_obj == "directive_lang_obj" } {
            # Directives are executed right here, right now.  They are not sent to the output.
            # Also, directive lines do not count as previous non-empty lines.

            if { [regexp "^\\${directive_char} *(\[^ \]+) *\$" $text -> new_lang] } {
               $top_frame set -cur_lang_obj [$this find_lang_obj $new_lang]
            } else {
               error "Invalid directive syntax."
            }

         } elseif { [regexp {^ *$} $text] } {
            # Empty line.

            if { $recognized_lang_obj == "prepro_lang_obj" } {
               # Output empty line in intermediate script.
               prepro_lang_obj output $fod $indent_prepro ""
            } else {
               # Decorate an empty line, to produce an empty line in the final output.
               prepro_lang_obj decorate $fod $indent_prepro "" ""
            }

         } else {
            # Non-empty, non-directive line.

            if { $recognized_lang_obj == "prepro_lang_obj" } {
               # The prepro lines are copied verbatim to the output.
               prepro_lang_obj output $fod $indent_prepro $text
            } else {
               # The other lines are first decorated by their language object.
               prepro_lang_obj decorate $fod $indent_prepro $indent_target $text
            }

            $this set -prev_non_empty_line $cur_line
         }
      }

      # Write a footer to the output file.
      prepro_lang_obj exit_file $fod

      # Cleanup.
      $this set -prev_non_empty_line ""

      # Delete all lines.
      foreach line [$this get -lines] {
         oblet_delete $line
      }

      # Do not delete the stack and the language objects here; we do that below.

      close $fid
      close $fod
   }

   ### Main method.
   proc go {infile outfile} {
      ### Start the stack.
      set top_frame [$this push_frame]
         # This establishes the toplevel indentation at "zero characters",
         # since the first line is required to start with the directive marker.

      ### Produce the intermediate script.
      $this produce_script $infile $outfile

      ### Produce the final output file.
      # Now that we know the prepro language (we found it in the input file!),
      # we know the correct extension for the intermediate script.
      set outfile1 "${outfile}.[prepro_lang_obj get -name]"
      file rename $outfile $outfile1

      # Simply run the intermediate script to produce the final output.
      prepro_lang_obj execute $outfile1 $outfile

      ### Cleanup.

      oblet_delete $top_frame
      $this set -stack [list] -prev_non_empty_line ""

      # Delete the language objects.
      oblet_delete directive_lang_obj prepro_lang_obj target_lang_obj
   }
}

if { [llength $argv] != 2 } {
   puts "Usage: [info script] <input file> <output file>"
   puts ""
   puts "Creates an intermediate script called '<output file>.extension'"
   puts "with the correct extension for the intermediate language."
   puts "Then executes that script to produce the final output."
   exit
}

LineProcessor main
main go [lindex $argv 0] [lindex $argv 1]

