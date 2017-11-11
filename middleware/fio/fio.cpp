#include "fio.h"
#include "string_utils.h"

static bool comment_line( const std::string&     contents,
                          const char             comment,
                          std::string::size_type ret,
                          std::string::size_type ret_nxt )
{
   for (std::string::size_type ind = ret + 1;
        ind < ret_nxt;
        ind++)
   {
      if (contents[ind] != ' ')
      {
         if (contents[ind] == comment) return true;
         return false;
      }
   }

   return false;
}

static std::string get_param_str( const std::string& contents,
                                  const std::string& parameter )
{
   std::string::size_type loc     = contents.find( parameter );
   std::string::size_type ret     = 0;
   std::string::size_type ret_nxt = contents.find( '\n' );

   /*
   ** return if the parameter is not found
   */
   if (loc == std::string::npos)
   {
      std::cout << "\"" << parameter << "\" parameter not found" << std::endl;
      return "";
   }

   /*
   ** search for the beginning and end of the line that
   ** contains the parameter that is being searched for.
   ** extract the parameter if it is not on a comment line.
   */
   while( ret_nxt < loc )
   {
      ret     = ret_nxt;
      ret_nxt = contents.find( '\n', ret + 1);
   }

   if( !comment_line( contents, '#', ret, ret_nxt ) )
   {

      std::string::size_type eq = contents.find( '=', loc );
      int ind;

      // advance past white space
      for( ind = eq + 1; contents[ind] == ' ' && ind < contents.length(); ind++) {}

      // build the number string
      std::string str_parm = "";
      for( ; contents[ind] != ' ' && ind < contents.length(); ind++)
      {
         str_parm += contents[ind];
      }

      // convert the number string to an integer
      return str_parm;
   }

   std::cout << "\"" << parameter << "\" parameter not found" << std::endl;
   return "";
}
                                  


namespace fio {

   /*
   ** constructor name: Parameter
   */
   Parameter::Parameter( std::string in_file )
   {
      std::ifstream input;
      input.open( in_file.c_str(), std::ios::in );

      // get length of file
      input.seekg( 0, input.end );
      int length = input.tellg();
      input.seekg( 0, input.beg );

      // allocate memory
      char* buffer = new char [length];

      // read all the data
      input.read( buffer, length );
      contents = char_to_string( buffer, length );
      delete[] buffer;

      // close file
      input.close();
   }  

   /*
   ** destructor name: Parameter
   */
   Parameter::~Parameter( void )
   {
      std::cout << "clearing parameter text-file-object contents" << std::endl;
   }

   /*
   ** function name: print_all from Parameter
   */
   void Parameter::print_all( void )
   {
      std::cout << contents;
   }

   /*
   ** function name: get_int from Parameter
   */
   int Parameter::get_int( const std::string& parameter )
   {
      std::string str_parm = get_param_str( contents, parameter );
      if ( str_parm.empty() ) return 0;
      return atoi( str_parm.c_str() );
   }

   // get_real function from Parameter class
   double Parameter::get_real( const std::string& parameter )
   {
      std::string str_parm = get_param_str( contents, parameter );
      if ( str_parm.empty() ) return 0.0;
      return atof( str_parm.c_str() );
   }
}  
