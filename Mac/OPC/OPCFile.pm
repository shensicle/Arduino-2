package OPCFile;
 
use strict;
use warnings;

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


# Defining hash keys here so we know what the fields are
our %ImageConfig = {Version => undef, ImageWidth => undef, ImageHeight => undef, BitDepth => undef,
                     ColourType => undef, Unused1 => undef, FilterMethod => undef, ScanType => undef };

# ==============================================================================
# init with file name
sub new
{
    my ($class,$fn) = @_;
  
    my $self = {_fileName => $fn};
    bless $self, $class
}

# ==============================================================================
# Open the file and validate the file header. Also read capture parameters from
# the image header
sub open
{
   my $self = shift; # object reference
 
   my $expectedHeader = pack('CCCCCCCC', 0x89, ord 'O', ord 'P', ord 'C', 0x0d, 0x0a, 0x1a, 0x0a);
   open  $self->{_fh}, '<:raw', $self->{_fileName};

   # OPC file header is 8 bytes
   my $bytes_read = read $self->{_fh}, my $bytes, 8;
   if ($bytes_read != 8) {
        die 'Unable to read OPC file header';
   }
   
   # Fail if the header is not intact
   die 'OPC file header is invalid' unless $bytes eq $expectedHeader;
   
   # Next read the image configuration
   $bytes_read = read $self->{_fh}, $bytes, 32;
   die 'Unable to read image configuration' unless $bytes_read == 32;
  
  
   ($ImageConfig{Version}, $ImageConfig{ImageWidth}, $ImageConfig{ImageHeight}, 
    $ImageConfig{BitDepth}, $ImageConfig{ColourType}, $ImageConfig{Unused1},
    $ImageConfig{FilterMethod}, $ImageConfig{ScanType})  = unpack( "IIIIIIII", "$bytes" );
    
   $self->{_imageConfig} = \%ImageConfig; 
    
   return %ImageConfig;
}

# ==============================================================================
sub get_rows
{
   my $self = shift; # object reference
   
   my $config = $self->{_imageConfig};
   my $width = $config->{ImageWidth};
   my $height = $config->{ImageHeight};
   my $depth = $config->{BitDepth};
   my @rowList;
  
   my $rowCount = 0;
   while ($rowCount < $height)
   {
       read $self->{_fh}, $rowList[$rowCount], $width*2;
       
       # Account for ZigZag encoding 
       if ((0 == $rowCount % 2)  && ($config->{ScanType} == 1))
       {
           $rowList[$rowCount] = scalar reverse $rowList[$rowCount]
       }
       $rowCount ++;
   }

   return @rowList;
}


# ==============================================================================
sub close
{
   my $self = shift; # object reference
   close  $self->{_fh};
}


1;