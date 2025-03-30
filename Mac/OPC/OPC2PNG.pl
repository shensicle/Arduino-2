#!/usr/bin/perl -w

#{
Copyright  2025 Scott Henwood/shensicle photographic. All Rights Reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products 
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Scott Henwood/shensicle photographic "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
}


use Image::PNG::Libpng ':all';
use Image::PNG::Const ':all';     # just in case
use File::Basename;

use OPCFile;

my $inputFile = $ARGV[0];


my $opcFile = new OPCFile ($inputFile);

# Open the OPC file and verify the file header and the image configuration header
my %imageConfig = $opcFile->open();

print "Image configuration is "; print "$_ $imageConfig{$_}\n" for keys %imageConfig; print "\n";

# Each row in the image is returned as a binary string in this array
@rows = $opcFile->get_rows();

# Done with the OPC file
$opcFile->close();

# Make an output file name from the input file name by replacing the .OPC extension with .png
my $outputFile = $inputFile;
$outputFile =~ s/\.OPC$/\.png/g;


# On to the PNG output
my $png = create_write_struct ();

my $ihdr = { width => $imageConfig{ImageWidth}, height => $imageConfig{ImageHeight}, bit_depth => $imageConfig{BitDepth},
             color_type => $imageConfig{ColourType} };

$png->set_IHDR ($ihdr);
$png->set_rows (\@rows);

open my $output, ">:raw", $outputFile;
$png->init_io ($output);
$png->write_png ();
close $output;

