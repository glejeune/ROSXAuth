#include <ruby.h>
#include <stdio.h>
#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>

typedef struct RbTROSXAuth {
	VALUE rbAuth;
	OSStatus myStatus; 
  AuthorizationRef myAuthorizationRef;
} RbTROSXAuth;

// Prototype for the initialization method
void Init_rosxauth();
VALUE cROSXAuth;
VALUE eROSXAuthError;

void rosxauth_free(RbTROSXAuth *);
void rosxauth_mark(RbTROSXAuth *);
VALUE rosxauth_new(VALUE);

VALUE rosxauth_status(VALUE);
VALUE rosxauth_auth(VALUE);
VALUE rosxauth_auth_q(VALUE);
VALUE rosxauth_exec( VALUE, VALUE, VALUE );