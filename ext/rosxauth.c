#include "rosxauth.h"

// The initialization method for this module
void Init_rosxauth() {
	cROSXAuth = rb_define_class("ROSXAuth", rb_cObject);
	eROSXAuthError = rb_define_class_under( cROSXAuth, "Error", rb_eRuntimeError );
	
	
	rb_define_singleton_method( cROSXAuth, "new", rosxauth_new, 0 );
	rb_define_method( cROSXAuth, "status", rosxauth_status,  0 );
	rb_define_method( cROSXAuth, "auth", rosxauth_auth, 0 );
	rb_define_method( cROSXAuth, "auth?", rosxauth_auth_q, 0 );
	rb_define_method( cROSXAuth, "exec", rosxauth_exec, 2 );
	
	rb_define_const( cROSXAuth, "ErrAuthorizationSuccess", INT2NUM( errAuthorizationSuccess ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationInvalidSet", INT2NUM( errAuthorizationInvalidSet ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationInvalidRef", INT2NUM( errAuthorizationInvalidRef ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationInvalidTag", INT2NUM( errAuthorizationInvalidTag ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationInvalidPointer", INT2NUM( errAuthorizationInvalidPointer ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationDenied", INT2NUM( errAuthorizationDenied ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationCanceled", INT2NUM( errAuthorizationCanceled ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationInteractionNotAllowed", INT2NUM( errAuthorizationInteractionNotAllowed ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationInternal", INT2NUM( errAuthorizationInternal ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationExternalizeNotAllowed", INT2NUM( errAuthorizationExternalizeNotAllowed ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationInternalizeNotAllowed", INT2NUM( errAuthorizationInternalizeNotAllowed ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationInvalidFlags", INT2NUM( errAuthorizationInvalidFlags ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationToolExecuteFailure", INT2NUM( errAuthorizationToolExecuteFailure ) );
	rb_define_const( cROSXAuth, "ErrAuthorizationToolEnvironmentError", INT2NUM( errAuthorizationToolEnvironmentError ) );
}

/* ------------------------------------------------------------------------------- */

void rosxauth_free( RbTROSXAuth *pRbTROSXAuth ) {
  if (pRbTROSXAuth != NULL)
    free(pRbTROSXAuth);
}

void rosxauth_mark( RbTROSXAuth *pRbTROSXAuth ) {
  if( pRbTROSXAuth == NULL ) return;
	AuthorizationFree(pRbTROSXAuth->myAuthorizationRef,kAuthorizationFlagDestroyRights);
  if( !NIL_P(pRbTROSXAuth->rbAuth) ) rb_gc_mark(pRbTROSXAuth->rbAuth);
}

/*
 * Create a new ROSXAuth object
 *
 * 	a = ROSXAuth.new()
 */
VALUE rosxauth_new( VALUE class ) {
  RbTROSXAuth *pRbTROSXAuth;

  pRbTROSXAuth = (RbTROSXAuth *)malloc(sizeof(RbTROSXAuth));
  if( pRbTROSXAuth == NULL )
    rb_raise(rb_eNoMemError, "No memory left for ROSXAuth struct");

  pRbTROSXAuth->rbAuth   = Qfalse;
	pRbTROSXAuth->myStatus = AuthorizationCreate(
      NULL,
      kAuthorizationEmptyEnvironment,
      kAuthorizationFlagDefaults,
      &pRbTROSXAuth->myAuthorizationRef);

  return( Data_Wrap_Struct( class, rosxauth_mark, rosxauth_free, pRbTROSXAuth ) );
}

/* ------------------------------------------------------------------------------- */

/*
 * Give the authorization status
 *
 * 	r = a.status
 */
VALUE rosxauth_status(VALUE self) {
	RbTROSXAuth *pRbTROSXAuth;
  Data_Get_Struct(self, RbTROSXAuth, pRbTROSXAuth);

	return INT2NUM(pRbTROSXAuth->myStatus);
}

/*
 * Authorizes and preauthorizes rights.
 *
 * 	a.auth
 */
VALUE rosxauth_auth(VALUE self) {
	RbTROSXAuth *pRbTROSXAuth;
  Data_Get_Struct(self, RbTROSXAuth, pRbTROSXAuth);

	if( pRbTROSXAuth->rbAuth == Qfalse )
	{ 
    AuthorizationItem myItems = {kAuthorizationRightExecute, 0, NULL, 0}; 
    AuthorizationRights myRights = {1, &myItems}; 
    AuthorizationFlags myFlags =
        kAuthorizationFlagDefaults | 
        kAuthorizationFlagInteractionAllowed | 
        kAuthorizationFlagPreAuthorize | 
        kAuthorizationFlagExtendRights;
  
    pRbTROSXAuth->myStatus = AuthorizationCopyRights(
        pRbTROSXAuth->myAuthorizationRef, &myRights, NULL, myFlags, NULL ); 
  }
	if( pRbTROSXAuth->myStatus == errAuthorizationSuccess ) {
		pRbTROSXAuth->rbAuth = Qtrue;
	}

	return INT2NUM(pRbTROSXAuth->myStatus);
}

/*
 * Check if autorizations are set
 */
VALUE rosxauth_auth_q(VALUE self) {
	RbTROSXAuth *pRbTROSXAuth;
  Data_Get_Struct(self, RbTROSXAuth, pRbTROSXAuth);
	
	return( pRbTROSXAuth->rbAuth );
}

/*
 * Runs an executable tool with root privileges.
 *
 * 	f = hh.exec( "/bin/ls", ["-l", "-a"] )
 *	if f.nil?
 *	  puts "i said NO !!!"
 *	else
 *	  IO.for_fd( f ).each do | g |
 *	    g.each_line { | l | puts l }
 *	  end
 *	end
 *
 * Return a file descriptor.
 */
VALUE rosxauth_exec( VALUE self, VALUE tool, VALUE args ) {
	char *cTool = STR2CSTR(tool);
	char **cArgs = NULL;
	FILE *execPipe = NULL; 
	int i, argsLen;
	
	RbTROSXAuth *pRbTROSXAuth;
  Data_Get_Struct(self, RbTROSXAuth, pRbTROSXAuth);

  // Auth if needed
	if( pRbTROSXAuth->rbAuth == Qfalse ) {
		VALUE r = rosxauth_auth( self );
		if( pRbTROSXAuth->rbAuth == Qfalse ) {
			return( Qnil );
		}
	}
	
	// Make args array
	switch(TYPE( args ) ) {
		case T_NIL:
			cArgs = NULL;
			break;
		
		case T_STRING:
			cArgs = (char**)malloc(sizeof(char*)*2);
			cArgs[0] = STR2CSTR(args);
			cArgs[1] = NULL;
			break;
		
		case T_ARRAY:
			argsLen = RARRAY(args)->len;
			cArgs = (char**)malloc(sizeof(char*)*(argsLen + 1));
			for( i = 0; i < argsLen; i++ ) {
				switch(TYPE(RARRAY(args)->ptr[i])) {
					case T_STRING:
						cArgs[i] = STR2CSTR(RARRAY(args)->ptr[i]);
						break;
					case T_FLOAT:
						cArgs[i] = (char*)malloc(sizeof(char)*255);
						sprintf( cArgs[i], "%f", NUM2DBL(RARRAY(args)->ptr[i]) );
						break;
					case T_FIXNUM:
					case T_BIGNUM:
						cArgs[i] = (char*)malloc(sizeof(char)*255);
						sprintf( cArgs[i], "%d", NUM2INT(RARRAY(args)->ptr[i]) );
						break;
					default:
						rb_raise( eROSXAuthError, "Parameter type not allowed !" );
						break;
				}
			}
			cArgs[argsLen] = NULL;
			break;
		
		default:
			break;
	}
	
	// Execute !
	pRbTROSXAuth->myStatus = AuthorizationExecuteWithPrivileges(
      pRbTROSXAuth->myAuthorizationRef,
      cTool,
      kAuthorizationFlagDefaults,
      cArgs,
      &execPipe);

	return INT2FIX(fileno(execPipe));
}