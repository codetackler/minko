// Copyright (c) 2014 Marshall A. Greenblatt. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the name Chromium Embedded
// Framework nor the names of its contributors may be used to endorse
// or promote products derived from this software without specific prior
// written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool and should not edited
// by hand. See the translator.README.txt file in the tools directory for
// more information.
//

#ifndef CEF_INCLUDE_CAPI_CEF_V8_CAPI_H_
#define CEF_INCLUDE_CAPI_CEF_V8_CAPI_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "include/capi/cef_base_capi.h"
#include "include/capi/cef_browser_capi.h"
#include "include/capi/cef_frame_capi.h"
#include "include/capi/cef_task_capi.h"

struct _cef_v8exception_t;
struct _cef_v8handler_t;
struct _cef_v8stack_frame_t;
struct _cef_v8value_t;

///
// Structure representing a V8 context handle. V8 handles can only be accessed
// from the thread on which they are created. Valid threads for creating a V8
// handle include the render process main thread (TID_RENDERER) and WebWorker
// threads. A task runner for posting tasks on the associated thread can be
// retrieved via the cef_v8context_t::get_task_runner() function.
///
typedef struct _cef_v8context_t {
  ///
  // Base structure.
  ///
  cef_base_t base;

  ///
  // Returns the task runner associated with this context. V8 handles can only
  // be accessed from the thread on which they are created. This function can be
  // called on any render process thread.
  ///
  struct _cef_task_runner_t* (CEF_CALLBACK *get_task_runner)(
      struct _cef_v8context_t* self);

  ///
  // Returns true (1) if the underlying handle is valid and it can be accessed
  // on the current thread. Do not call any other functions if this function
  // returns false (0).
  ///
  int (CEF_CALLBACK *is_valid)(struct _cef_v8context_t* self);

  ///
  // Returns the browser for this context. This function will return an NULL
  // reference for WebWorker contexts.
  ///
  struct _cef_browser_t* (CEF_CALLBACK *get_browser)(
      struct _cef_v8context_t* self);

  ///
  // Returns the frame for this context. This function will return an NULL
  // reference for WebWorker contexts.
  ///
  struct _cef_frame_t* (CEF_CALLBACK *get_frame)(struct _cef_v8context_t* self);

  ///
  // Returns the global object for this context. The context must be entered
  // before calling this function.
  ///
  struct _cef_v8value_t* (CEF_CALLBACK *get_global)(
      struct _cef_v8context_t* self);

  ///
  // Enter this context. A context must be explicitly entered before creating a
  // V8 Object, Array, Function or Date asynchronously. exit() must be called
  // the same number of times as enter() before releasing this context. V8
  // objects belong to the context in which they are created. Returns true (1)
  // if the scope was entered successfully.
  ///
  int (CEF_CALLBACK *enter)(struct _cef_v8context_t* self);

  ///
  // Exit this context. Call this function only after calling enter(). Returns
  // true (1) if the scope was exited successfully.
  ///
  int (CEF_CALLBACK *exit)(struct _cef_v8context_t* self);

  ///
  // Returns true (1) if this object is pointing to the same handle as |that|
  // object.
  ///
  int (CEF_CALLBACK *is_same)(struct _cef_v8context_t* self,
      struct _cef_v8context_t* that);

  ///
  // Evaluates the specified JavaScript code using this context's global object.
  // On success |retval| will be set to the return value, if any, and the
  // function will return true (1). On failure |exception| will be set to the
  // exception, if any, and the function will return false (0).
  ///
  int (CEF_CALLBACK *eval)(struct _cef_v8context_t* self,
      const cef_string_t* code, struct _cef_v8value_t** retval,
      struct _cef_v8exception_t** exception);
} cef_v8context_t;


///
// Returns the current (top) context object in the V8 context stack.
///
CEF_EXPORT cef_v8context_t* cef_v8context_get_current_context();

///
// Returns the entered (bottom) context object in the V8 context stack.
///
CEF_EXPORT cef_v8context_t* cef_v8context_get_entered_context();

///
// Returns true (1) if V8 is currently inside a context.
///
CEF_EXPORT int cef_v8context_in_context();


///
// Structure that should be implemented to handle V8 function calls. The
// functions of this structure will be called on the thread associated with the
// V8 function.
///
typedef struct _cef_v8handler_t {
  ///
  // Base structure.
  ///
  cef_base_t base;

  ///
  // Handle execution of the function identified by |name|. |object| is the
  // receiver ('this' object) of the function. |arguments| is the list of
  // arguments passed to the function. If execution succeeds set |retval| to the
  // function return value. If execution fails set |exception| to the exception
  // that will be thrown. Return true (1) if execution was handled.
  ///
  int (CEF_CALLBACK *execute)(struct _cef_v8handler_t* self,
      const cef_string_t* name, struct _cef_v8value_t* object,
      size_t argumentsCount, struct _cef_v8value_t* const* arguments,
      struct _cef_v8value_t** retval, cef_string_t* exception);
} cef_v8handler_t;


///
// Structure that should be implemented to handle V8 accessor calls. Accessor
// identifiers are registered by calling cef_v8value_t::set_value_byaccessor().
// The functions of this structure will be called on the thread associated with
// the V8 accessor.
///
typedef struct _cef_v8accessor_t {
  ///
  // Base structure.
  ///
  cef_base_t base;

  ///
  // Handle retrieval the accessor value identified by |name|. |object| is the
  // receiver ('this' object) of the accessor. If retrieval succeeds set
  // |retval| to the return value. If retrieval fails set |exception| to the
  // exception that will be thrown. Return true (1) if accessor retrieval was
  // handled.
  ///
  int (CEF_CALLBACK *get)(struct _cef_v8accessor_t* self,
      const cef_string_t* name, struct _cef_v8value_t* object,
      struct _cef_v8value_t** retval, cef_string_t* exception);

  ///
  // Handle assignment of the accessor value identified by |name|. |object| is
  // the receiver ('this' object) of the accessor. |value| is the new value
  // being assigned to the accessor. If assignment fails set |exception| to the
  // exception that will be thrown. Return true (1) if accessor assignment was
  // handled.
  ///
  int (CEF_CALLBACK *set)(struct _cef_v8accessor_t* self,
      const cef_string_t* name, struct _cef_v8value_t* object,
      struct _cef_v8value_t* value, cef_string_t* exception);
} cef_v8accessor_t;


///
// Structure representing a V8 exception. The functions of this structure may be
// called on any render process thread.
///
typedef struct _cef_v8exception_t {
  ///
  // Base structure.
  ///
  cef_base_t base;

  ///
  // Returns the exception message.
  ///
  // The resulting string must be freed by calling cef_string_userfree_free().
  cef_string_userfree_t (CEF_CALLBACK *get_message)(
      struct _cef_v8exception_t* self);

  ///
  // Returns the line of source code that the exception occurred within.
  ///
  // The resulting string must be freed by calling cef_string_userfree_free().
  cef_string_userfree_t (CEF_CALLBACK *get_source_line)(
      struct _cef_v8exception_t* self);

  ///
  // Returns the resource name for the script from where the function causing
  // the error originates.
  ///
  // The resulting string must be freed by calling cef_string_userfree_free().
  cef_string_userfree_t (CEF_CALLBACK *get_script_resource_name)(
      struct _cef_v8exception_t* self);

  ///
  // Returns the 1-based number of the line where the error occurred or 0 if the
  // line number is unknown.
  ///
  int (CEF_CALLBACK *get_line_number)(struct _cef_v8exception_t* self);

  ///
  // Returns the index within the script of the first character where the error
  // occurred.
  ///
  int (CEF_CALLBACK *get_start_position)(struct _cef_v8exception_t* self);

  ///
  // Returns the index within the script of the last character where the error
  // occurred.
  ///
  int (CEF_CALLBACK *get_end_position)(struct _cef_v8exception_t* self);

  ///
  // Returns the index within the line of the first character where the error
  // occurred.
  ///
  int (CEF_CALLBACK *get_start_column)(struct _cef_v8exception_t* self);

  ///
  // Returns the index within the line of the last character where the error
  // occurred.
  ///
  int (CEF_CALLBACK *get_end_column)(struct _cef_v8exception_t* self);
} cef_v8exception_t;


///
// Structure representing a V8 value handle. V8 handles can only be accessed
// from the thread on which they are created. Valid threads for creating a V8
// handle include the render process main thread (TID_RENDERER) and WebWorker
// threads. A task runner for posting tasks on the associated thread can be
// retrieved via the cef_v8context_t::get_task_runner() function.
///
typedef struct _cef_v8value_t {
  ///
  // Base structure.
  ///
  cef_base_t base;

  ///
  // Returns true (1) if the underlying handle is valid and it can be accessed
  // on the current thread. Do not call any other functions if this function
  // returns false (0).
  ///
  int (CEF_CALLBACK *is_valid)(struct _cef_v8value_t* self);

  ///
  // True if the value type is undefined.
  ///
  int (CEF_CALLBACK *is_undefined)(struct _cef_v8value_t* self);

  ///
  // True if the value type is null.
  ///
  int (CEF_CALLBACK *is_null)(struct _cef_v8value_t* self);

  ///
  // True if the value type is bool.
  ///
  int (CEF_CALLBACK *is_bool)(struct _cef_v8value_t* self);

  ///
  // True if the value type is int.
  ///
  int (CEF_CALLBACK *is_int)(struct _cef_v8value_t* self);

  ///
  // True if the value type is unsigned int.
  ///
  int (CEF_CALLBACK *is_uint)(struct _cef_v8value_t* self);

  ///
  // True if the value type is double.
  ///
  int (CEF_CALLBACK *is_double)(struct _cef_v8value_t* self);

  ///
  // True if the value type is Date.
  ///
  int (CEF_CALLBACK *is_date)(struct _cef_v8value_t* self);

  ///
  // True if the value type is string.
  ///
  int (CEF_CALLBACK *is_string)(struct _cef_v8value_t* self);

  ///
  // True if the value type is object.
  ///
  int (CEF_CALLBACK *is_object)(struct _cef_v8value_t* self);

  ///
  // True if the value type is array.
  ///
  int (CEF_CALLBACK *is_array)(struct _cef_v8value_t* self);

  ///
  // True if the value type is function.
  ///
  int (CEF_CALLBACK *is_function)(struct _cef_v8value_t* self);

  ///
  // Returns true (1) if this object is pointing to the same handle as |that|
  // object.
  ///
  int (CEF_CALLBACK *is_same)(struct _cef_v8value_t* self,
      struct _cef_v8value_t* that);

  ///
  // Return a bool value.  The underlying data will be converted to if
  // necessary.
  ///
  int (CEF_CALLBACK *get_bool_value)(struct _cef_v8value_t* self);

  ///
  // Return an int value.  The underlying data will be converted to if
  // necessary.
  ///
  int32 (CEF_CALLBACK *get_int_value)(struct _cef_v8value_t* self);

  ///
  // Return an unisgned int value.  The underlying data will be converted to if
  // necessary.
  ///
  uint32 (CEF_CALLBACK *get_uint_value)(struct _cef_v8value_t* self);

  ///
  // Return a double value.  The underlying data will be converted to if
  // necessary.
  ///
  double (CEF_CALLBACK *get_double_value)(struct _cef_v8value_t* self);

  ///
  // Return a Date value.  The underlying data will be converted to if
  // necessary.
  ///
  cef_time_t (CEF_CALLBACK *get_date_value)(struct _cef_v8value_t* self);

  ///
  // Return a string value.  The underlying data will be converted to if
  // necessary.
  ///
  // The resulting string must be freed by calling cef_string_userfree_free().
  cef_string_userfree_t (CEF_CALLBACK *get_string_value)(
      struct _cef_v8value_t* self);


  // OBJECT METHODS - These functions are only available on objects. Arrays and
  // functions are also objects. String- and integer-based keys can be used
  // interchangably with the framework converting between them as necessary.

  ///
  // Returns true (1) if this is a user created object.
  ///
  int (CEF_CALLBACK *is_user_created)(struct _cef_v8value_t* self);

  ///
  // Returns true (1) if the last function call resulted in an exception. This
  // attribute exists only in the scope of the current CEF value object.
  ///
  int (CEF_CALLBACK *has_exception)(struct _cef_v8value_t* self);

  ///
  // Returns the exception resulting from the last function call. This attribute
  // exists only in the scope of the current CEF value object.
  ///
  struct _cef_v8exception_t* (CEF_CALLBACK *get_exception)(
      struct _cef_v8value_t* self);

  ///
  // Clears the last exception and returns true (1) on success.
  ///
  int (CEF_CALLBACK *clear_exception)(struct _cef_v8value_t* self);

  ///
  // Returns true (1) if this object will re-throw future exceptions. This
  // attribute exists only in the scope of the current CEF value object.
  ///
  int (CEF_CALLBACK *will_rethrow_exceptions)(struct _cef_v8value_t* self);

  ///
  // Set whether this object will re-throw future exceptions. By default
  // exceptions are not re-thrown. If a exception is re-thrown the current
  // context should not be accessed again until after the exception has been
  // caught and not re-thrown. Returns true (1) on success. This attribute
  // exists only in the scope of the current CEF value object.
  ///
  int (CEF_CALLBACK *set_rethrow_exceptions)(struct _cef_v8value_t* self,
      int rethrow);

  ///
  // Returns true (1) if the object has a value with the specified identifier.
  ///
  int (CEF_CALLBACK *has_value_bykey)(struct _cef_v8value_t* self,
      const cef_string_t* key);

  ///
  // Returns true (1) if the object has a value with the specified identifier.
  ///
  int (CEF_CALLBACK *has_value_byindex)(struct _cef_v8value_t* self, int index);

  ///
  // Deletes the value with the specified identifier and returns true (1) on
  // success. Returns false (0) if this function is called incorrectly or an
  // exception is thrown. For read-only and don't-delete values this function
  // will return true (1) even though deletion failed.
  ///
  int (CEF_CALLBACK *delete_value_bykey)(struct _cef_v8value_t* self,
      const cef_string_t* key);

  ///
  // Deletes the value with the specified identifier and returns true (1) on
  // success. Returns false (0) if this function is called incorrectly, deletion
  // fails or an exception is thrown. For read-only and don't-delete values this
  // function will return true (1) even though deletion failed.
  ///
  int (CEF_CALLBACK *delete_value_byindex)(struct _cef_v8value_t* self,
      int index);

  ///
  // Returns the value with the specified identifier on success. Returns NULL if
  // this function is called incorrectly or an exception is thrown.
  ///
  struct _cef_v8value_t* (CEF_CALLBACK *get_value_bykey)(
      struct _cef_v8value_t* self, const cef_string_t* key);

  ///
  // Returns the value with the specified identifier on success. Returns NULL if
  // this function is called incorrectly or an exception is thrown.
  ///
  struct _cef_v8value_t* (CEF_CALLBACK *get_value_byindex)(
      struct _cef_v8value_t* self, int index);

  ///
  // Associates a value with the specified identifier and returns true (1) on
  // success. Returns false (0) if this function is called incorrectly or an
  // exception is thrown. For read-only values this function will return true
  // (1) even though assignment failed.
  ///
  int (CEF_CALLBACK *set_value_bykey)(struct _cef_v8value_t* self,
      const cef_string_t* key, struct _cef_v8value_t* value,
      cef_v8_propertyattribute_t attribute);

  ///
  // Associates a value with the specified identifier and returns true (1) on
  // success. Returns false (0) if this function is called incorrectly or an
  // exception is thrown. For read-only values this function will return true
  // (1) even though assignment failed.
  ///
  int (CEF_CALLBACK *set_value_byindex)(struct _cef_v8value_t* self, int index,
      struct _cef_v8value_t* value);

  ///
  // Registers an identifier and returns true (1) on success. Access to the
  // identifier will be forwarded to the cef_v8accessor_t instance passed to
  // cef_v8value_t::cef_v8value_create_object(). Returns false (0) if this
  // function is called incorrectly or an exception is thrown. For read-only
  // values this function will return true (1) even though assignment failed.
  ///
  int (CEF_CALLBACK *set_value_byaccessor)(struct _cef_v8value_t* self,
      const cef_string_t* key, cef_v8_accesscontrol_t settings,
      cef_v8_propertyattribute_t attribute);

  ///
  // Read the keys for the object's values into the specified vector. Integer-
  // based keys will also be returned as strings.
  ///
  int (CEF_CALLBACK *get_keys)(struct _cef_v8value_t* self,
      cef_string_list_t keys);

  ///
  // Sets the user data for this object and returns true (1) on success. Returns
  // false (0) if this function is called incorrectly. This function can only be
  // called on user created objects.
  ///
  int (CEF_CALLBACK *set_user_data)(struct _cef_v8value_t* self,
      struct _cef_base_t* user_data);

  ///
  // Returns the user data, if any, assigned to this object.
  ///
  struct _cef_base_t* (CEF_CALLBACK *get_user_data)(
      struct _cef_v8value_t* self);

  ///
  // Returns the amount of externally allocated memory registered for the
  // object.
  ///
  int (CEF_CALLBACK *get_externally_allocated_memory)(
      struct _cef_v8value_t* self);

  ///
  // Adjusts the amount of registered external memory for the object. Used to
  // give V8 an indication of the amount of externally allocated memory that is
  // kept alive by JavaScript objects. V8 uses this information to decide when
  // to perform global garbage collection. Each cef_v8value_t tracks the amount
  // of external memory associated with it and automatically decreases the
  // global total by the appropriate amount on its destruction.
  // |change_in_bytes| specifies the number of bytes to adjust by. This function
  // returns the number of bytes associated with the object after the
  // adjustment. This function can only be called on user created objects.
  ///
  int (CEF_CALLBACK *adjust_externally_allocated_memory)(
      struct _cef_v8value_t* self, int change_in_bytes);


  // ARRAY METHODS - These functions are only available on arrays.

  ///
  // Returns the number of elements in the array.
  ///
  int (CEF_CALLBACK *get_array_length)(struct _cef_v8value_t* self);


  // FUNCTION METHODS - These functions are only available on functions.

  ///
  // Returns the function name.
  ///
  // The resulting string must be freed by calling cef_string_userfree_free().
  cef_string_userfree_t (CEF_CALLBACK *get_function_name)(
      struct _cef_v8value_t* self);

  ///
  // Returns the function handler or NULL if not a CEF-created function.
  ///
  struct _cef_v8handler_t* (CEF_CALLBACK *get_function_handler)(
      struct _cef_v8value_t* self);

  ///
  // Execute the function using the current V8 context. This function should
  // only be called from within the scope of a cef_v8handler_t or
  // cef_v8accessor_t callback, or in combination with calling enter() and
  // exit() on a stored cef_v8context_t reference. |object| is the receiver
  // ('this' object) of the function. If |object| is NULL the current context's
  // global object will be used. |arguments| is the list of arguments that will
  // be passed to the function. Returns the function return value on success.
  // Returns NULL if this function is called incorrectly or an exception is
  // thrown.
  ///
  struct _cef_v8value_t* (CEF_CALLBACK *execute_function)(
      struct _cef_v8value_t* self, struct _cef_v8value_t* object,
      size_t argumentsCount, struct _cef_v8value_t* const* arguments);

  ///
  // Execute the function using the specified V8 context. |object| is the
  // receiver ('this' object) of the function. If |object| is NULL the specified
  // context's global object will be used. |arguments| is the list of arguments
  // that will be passed to the function. Returns the function return value on
  // success. Returns NULL if this function is called incorrectly or an
  // exception is thrown.
  ///
  struct _cef_v8value_t* (CEF_CALLBACK *execute_function_with_context)(
      struct _cef_v8value_t* self, struct _cef_v8context_t* context,
      struct _cef_v8value_t* object, size_t argumentsCount,
      struct _cef_v8value_t* const* arguments);
} cef_v8value_t;


///
// Create a new cef_v8value_t object of type undefined.
///
CEF_EXPORT cef_v8value_t* cef_v8value_create_undefined();

///
// Create a new cef_v8value_t object of type null.
///
CEF_EXPORT cef_v8value_t* cef_v8value_create_null();

///
// Create a new cef_v8value_t object of type bool.
///
CEF_EXPORT cef_v8value_t* cef_v8value_create_bool(int value);

///
// Create a new cef_v8value_t object of type int.
///
CEF_EXPORT cef_v8value_t* cef_v8value_create_int(int32 value);

///
// Create a new cef_v8value_t object of type unsigned int.
///
CEF_EXPORT cef_v8value_t* cef_v8value_create_uint(uint32 value);

///
// Create a new cef_v8value_t object of type double.
///
CEF_EXPORT cef_v8value_t* cef_v8value_create_double(double value);

///
// Create a new cef_v8value_t object of type Date. This function should only be
// called from within the scope of a cef_v8context_tHandler, cef_v8handler_t or
// cef_v8accessor_t callback, or in combination with calling enter() and exit()
// on a stored cef_v8context_t reference.
///
CEF_EXPORT cef_v8value_t* cef_v8value_create_date(const cef_time_t* date);

///
// Create a new cef_v8value_t object of type string.
///
CEF_EXPORT cef_v8value_t* cef_v8value_create_string(const cef_string_t* value);

///
// Create a new cef_v8value_t object of type object with optional accessor. This
// function should only be called from within the scope of a
// cef_v8context_tHandler, cef_v8handler_t or cef_v8accessor_t callback, or in
// combination with calling enter() and exit() on a stored cef_v8context_t
// reference.
///
CEF_EXPORT cef_v8value_t* cef_v8value_create_object(cef_v8accessor_t* accessor);

///
// Create a new cef_v8value_t object of type array with the specified |length|.
// If |length| is negative the returned array will have length 0. This function
// should only be called from within the scope of a cef_v8context_tHandler,
// cef_v8handler_t or cef_v8accessor_t callback, or in combination with calling
// enter() and exit() on a stored cef_v8context_t reference.
///
CEF_EXPORT cef_v8value_t* cef_v8value_create_array(int length);

///
// Create a new cef_v8value_t object of type function. This function should only
// be called from within the scope of a cef_v8context_tHandler, cef_v8handler_t
// or cef_v8accessor_t callback, or in combination with calling enter() and
// exit() on a stored cef_v8context_t reference.
///
CEF_EXPORT cef_v8value_t* cef_v8value_create_function(const cef_string_t* name,
    cef_v8handler_t* handler);


///
// Structure representing a V8 stack trace handle. V8 handles can only be
// accessed from the thread on which they are created. Valid threads for
// creating a V8 handle include the render process main thread (TID_RENDERER)
// and WebWorker threads. A task runner for posting tasks on the associated
// thread can be retrieved via the cef_v8context_t::get_task_runner() function.
///
typedef struct _cef_v8stack_trace_t {
  ///
  // Base structure.
  ///
  cef_base_t base;

  ///
  // Returns true (1) if the underlying handle is valid and it can be accessed
  // on the current thread. Do not call any other functions if this function
  // returns false (0).
  ///
  int (CEF_CALLBACK *is_valid)(struct _cef_v8stack_trace_t* self);

  ///
  // Returns the number of stack frames.
  ///
  int (CEF_CALLBACK *get_frame_count)(struct _cef_v8stack_trace_t* self);

  ///
  // Returns the stack frame at the specified 0-based index.
  ///
  struct _cef_v8stack_frame_t* (CEF_CALLBACK *get_frame)(
      struct _cef_v8stack_trace_t* self, int index);
} cef_v8stack_trace_t;


///
// Returns the stack trace for the currently active context. |frame_limit| is
// the maximum number of frames that will be captured.
///
CEF_EXPORT cef_v8stack_trace_t* cef_v8stack_trace_get_current(int frame_limit);


///
// Structure representing a V8 stack frame handle. V8 handles can only be
// accessed from the thread on which they are created. Valid threads for
// creating a V8 handle include the render process main thread (TID_RENDERER)
// and WebWorker threads. A task runner for posting tasks on the associated
// thread can be retrieved via the cef_v8context_t::get_task_runner() function.
///
typedef struct _cef_v8stack_frame_t {
  ///
  // Base structure.
  ///
  cef_base_t base;

  ///
  // Returns true (1) if the underlying handle is valid and it can be accessed
  // on the current thread. Do not call any other functions if this function
  // returns false (0).
  ///
  int (CEF_CALLBACK *is_valid)(struct _cef_v8stack_frame_t* self);

  ///
  // Returns the name of the resource script that contains the function.
  ///
  // The resulting string must be freed by calling cef_string_userfree_free().
  cef_string_userfree_t (CEF_CALLBACK *get_script_name)(
      struct _cef_v8stack_frame_t* self);

  ///
  // Returns the name of the resource script that contains the function or the
  // sourceURL value if the script name is undefined and its source ends with a
  // "//@ sourceURL=..." string.
  ///
  // The resulting string must be freed by calling cef_string_userfree_free().
  cef_string_userfree_t (CEF_CALLBACK *get_script_name_or_source_url)(
      struct _cef_v8stack_frame_t* self);

  ///
  // Returns the name of the function.
  ///
  // The resulting string must be freed by calling cef_string_userfree_free().
  cef_string_userfree_t (CEF_CALLBACK *get_function_name)(
      struct _cef_v8stack_frame_t* self);

  ///
  // Returns the 1-based line number for the function call or 0 if unknown.
  ///
  int (CEF_CALLBACK *get_line_number)(struct _cef_v8stack_frame_t* self);

  ///
  // Returns the 1-based column offset on the line for the function call or 0 if
  // unknown.
  ///
  int (CEF_CALLBACK *get_column)(struct _cef_v8stack_frame_t* self);

  ///
  // Returns true (1) if the function was compiled using eval().
  ///
  int (CEF_CALLBACK *is_eval)(struct _cef_v8stack_frame_t* self);

  ///
  // Returns true (1) if the function was called as a constructor via "new".
  ///
  int (CEF_CALLBACK *is_constructor)(struct _cef_v8stack_frame_t* self);
} cef_v8stack_frame_t;


///
// Register a new V8 extension with the specified JavaScript extension code and
// handler. Functions implemented by the handler are prototyped using the
// keyword 'native'. The calling of a native function is restricted to the scope
// in which the prototype of the native function is defined. This function may
// only be called on the render process main thread.
//
// Example JavaScript extension code: <pre>
//   // create the 'example' global object if it doesn't already exist.
//   if (!example)
//     example = {};
//   // create the 'example.test' global object if it doesn't already exist.
//   if (!example.test)
//     example.test = {};
//   (function() {
//     // Define the function 'example.test.myfunction'.
//     example.test.myfunction = function() {
//       // Call CefV8Handler::Execute() with the function name 'MyFunction'
//       // and no arguments.
//       native function MyFunction();
//       return MyFunction();
//     };
//     // Define the getter function for parameter 'example.test.myparam'.
//     example.test.__defineGetter__('myparam', function() {
//       // Call CefV8Handler::Execute() with the function name 'GetMyParam'
//       // and no arguments.
//       native function GetMyParam();
//       return GetMyParam();
//     });
//     // Define the setter function for parameter 'example.test.myparam'.
//     example.test.__defineSetter__('myparam', function(b) {
//       // Call CefV8Handler::Execute() with the function name 'SetMyParam'
//       // and a single argument.
//       native function SetMyParam();
//       if(b) SetMyParam(b);
//     });
//
//     // Extension definitions can also contain normal JavaScript variables
//     // and functions.
//     var myint = 0;
//     example.test.increment = function() {
//       myint += 1;
//       return myint;
//     };
//   })();
// </pre> Example usage in the page: <pre>
//   // Call the function.
//   example.test.myfunction();
//   // Set the parameter.
//   example.test.myparam = value;
//   // Get the parameter.
//   value = example.test.myparam;
//   // Call another function.
//   example.test.increment();
// </pre>
///
CEF_EXPORT int cef_register_extension(const cef_string_t* extension_name,
    const cef_string_t* javascript_code, cef_v8handler_t* handler);

#ifdef __cplusplus
}
#endif

#endif  // CEF_INCLUDE_CAPI_CEF_V8_CAPI_H_
