# gzipHtmlPages.py - Script to gzip html files and create data arrays  

# MIT License

# Copyright (c) 2022 Pagliarulo Onofrio (oponyx@hotmail.it)

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.



Import('env')

import os
import gzip
import shutil
import glob
import pathlib
import sys

html_path =  os.path.join(pathlib.Path(env["PROJECT_DATA_DIR"]), 'html')
output_path =   os.path.abspath(pathlib.Path(env["PROJECT_SRC_DIR"]))
project_build_dir =   os.path.abspath(pathlib.Path(env.subst("$BUILD_DIR")))
binary_file_name = os.path.join(project_build_dir, 'firmware.bin')
#print( 'html_path:'+ html_path)
#print( 'output_path:'+ output_path)
#print( 'binary_file_name:'+ binary_file_name)
this_script_name =  os.path.basename(sys.argv[0])
print(this_script_name)

# gzip single file
def gzip_file( input_file_path, output_file_path ):
    #print('gzip_file: input_file_path:' + input_file_path + ' output_file_path:' + output_file_path)
    # write gzip file
    with open(input_file_path, "rb") as in_f:
        with gzip.open(output_file_path, "wb", compresslevel=9) as out_f:
            shutil.copyfileobj(in_f, out_f)


# create include file from gz
def build_header_file( input_file_path,  output_file_path):
    #print('build_header_file: input_file_path:' + input_file_path + ' output_file_path:' + output_file_path)

    total_count = 0
    line_chars_count = 0
    fname = pathlib.Path(input_file_path).stem
    define_name = fname.replace('.' , '_')
    #print( 'CONVERT_HTML: output_file_path: ' + output_file_path )
    try:
        with open(output_file_path, "w") as fo:
            fo.write('#ifndef _' + define_name + '_\n')
            fo.write('#define _' + define_name + '_\n\n')
            array_size = os.path.getsize(input_file_path)
            fo.write('const size_t ' + define_name +'_len = ' + str(array_size) + ';\n')
            fo.write('const char ' + define_name + '[] PROGMEM =\n')
            fo.write('"')

        
            with open(input_file_path, "rb") as fi:
                byte = fi.read(1)
                while byte:
                    fo.write(''.join(['\\x%02X' % b for b in byte]))
                    byte = fi.read(1)
                    line_chars_count += 1
                    total_count +=1
                    if line_chars_count == 16:
                        line_chars_count =0
                        fo.write('"\n"')
            
            fo.write('";\n\n')
            fo.write('#endif\n')
            #print('File size:' + str(array_size) + ' total_count:' + str(total_count))
    except IOError:
        print('Error While Opening the file!')  




    
    

# convert files
def convert_html_files( html_path, output_path, env ):
    total_bytes = 0
    print( '\nCONVERT HTML FILES TO CHAR ARRAY STARTED\n')
    # file types to be compressed..
    filetypes_to_process = [ 'html' ] # ,'css'

    # CHECK INPUT DIR
    if not os.path.exists( html_path ):
        print( 'CONVERT_HTML: HTML DIRECTORY NOT FOUND: ' + html_path )
        print( 'CONVERT_HTML: ABORTING...' )
        return
    
    # CHECK OUTPUT DIR
    if not os.path.exists( output_path ):
        print( 'output_path: OUTPUT DIRECTORY NOT FOUND: ' + output_path )
        print( 'CONVERT_HTML: ABORTING...' )
        return

    # FILES TO PROCESS
    files_to_process = []
    for extension in filetypes_to_process:
        files_to_process.extend( glob.glob( os.path.join( html_path, '*.' + extension ) ) )
    
    #print( 'CONVERT_HTML: PROCESSING FILES... {}\n'.format( files_to_process ) )

    # PROCESS FILES
    process_error = False
    try:
        for processing_file_path in files_to_process:
            gz_file_path = os.path.join( output_path, os.path.basename( processing_file_path ) + '.gz' )
            fname = pathlib.Path(gz_file_path).stem
            output_header_file_path = os.path.join( output_path,fname + '.h')
            if os.path.exists( binary_file_name ):
                if os.stat(binary_file_name).st_mtime - os.stat(processing_file_path).st_mtime >1:
                    print( 'CONVERT_HTML: File ' + output_header_file_path + ' seems unchanged. Skipping' )
                    continue
            #print('gz file:' + gz_file_path + ' output header:' + output_header_file_path)
            #dir = os.path.dirname(processing_file_path)
            #output_header_file_path = dir + '\\' + fname + '.h'
            print( 'CONVERT_HTML: PROCESSING FILE ' + processing_file_path )
            # CHECK IF FILE ALREADY EXISTS
            if os.path.exists( gz_file_path ):
                print( 'CONVERT_HTML: REMOVING... ' + gz_file_path )
                os.remove( gz_file_path )

            gzip_file( processing_file_path, gz_file_path ) 
            #print( 'CONVERT_HTML: GZIPPED... ' + gz_file_path + "\n" )
            build_header_file(gz_file_path, output_header_file_path) #generate the header file
            inp_file_size = os.path.getsize(processing_file_path)
            gz_file_size = os.path.getsize(gz_file_path)
            red = inp_file_size - gz_file_size
            #print( 'CONVERT_HTML: size in: ' + str(inp_file_size) + ' size out:' + str(gz_file_size) + ' reduced:' + str(red))
            total_bytes = total_bytes + red
            os.remove( gz_file_path ) #delete the gzip file

    except IOError as e:
        process_error = True
        print( 'CONVERT_HTML: FAILED TO PROCESS FILE ' + processing_file_path )
        # print( 'GZIP: EXCEPTION... {}'.format( e ) )
    if process_error:
        print( '\n<<<<<<<<<<< CONVERT_HTML: ERROR! >>>>>>>>>>>\n' )
    else:
        print( '\nCONVERT_HTML: FINISH.' )
        print( 'CONVERT_HTML: Bytes red: '+ str(total_bytes) + '\n')

# call the routine
convert_html_files(html_path, output_path, env)