## ===== instance function implementation template
static JSValue ${signature_name}(JSContext* cx, JSValueConst thisv, int argc, JSValueConst *argv)
{
#if len($arguments) > 0
    bool ok = true;
#end if
#if not $is_constructor
    ${namespaced_class_name}* cobj = JS_GetOpaque2(cx, thisv, jsb_${underlined_class_name}_class_id);
    JSB_PRECONDITION2( cobj, cx, JS_EXCEPTION, "${signature_name} : Invalid Native Object");
#end if
#if len($arguments) >= $min_args
    #set arg_count = len($arguments)
    #set arg_idx = $min_args
    #while $arg_idx <= $arg_count
    if (argc == ${arg_idx}) {
        #set $count = 0
        #while $count < $arg_idx
            #set $arg = $arguments[$count]
            #if $arg.is_numeric
        ${arg.to_string($generator)} arg${count} = 0;
            #elif $arg.is_pointer
        ${arg.to_string($generator)} arg${count} = nullptr;
            #else
        ${arg.to_string($generator)} arg${count};
            #end if
            #set $count = $count + 1
        #end while
        #set $count = 0
        #set arg_list = ""
        #set arg_array = []
        #while $count < $arg_idx
            #set $arg = $arguments[$count]
        ${arg.to_native({"generator": $generator,
                             "in_value": "args.get(" + str(count) + ")",
                             "out_value": "arg" + str(count),
                             "class_name": $class_name,
                             "level": 2,
                             "ntype": str($arg)})};
            #set $arg_array += ["arg"+str(count)]
            #set $count = $count + 1
        #end while
        #if $arg_idx > 0
        JSB_PRECONDITION2(ok, cx, JS_EXCEPTION, "${signature_name} : Error processing arguments");
        #end if
        #set $arg_list = ", ".join($arg_array)
        #if $is_constructor
        ${namespaced_class_name}* cobj = new (std::nothrow) ${namespaced_class_name}($arg_list);

        js_type_class_t *typeClass = js_get_type_from_native<${namespaced_class_name}>(cobj);
        JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "${namespaced_class_name}"));
        args.rval().set(OBJECT_TO_JSVAL(jsobj));
        #else
            #if $ret_type.name != "void"
                #if $ret_type.is_enum
        int ret = (int)cobj->${func_name}($arg_list);
                #else
        ${ret_type.get_whole_name($generator)} ret = cobj->${func_name}($arg_list);
                #end if
        return ${ret_type.from_native({"generator": $generator,
                                    "in_value": "ret",
                                    "out_value": "jsret",
                                    "ntype": str($ret_type),
                                    "level": 2})};
            #else
        cobj->${func_name}($arg_list);
        return JS_UNDEFINED;
            #end if
        #end if
    }
        #set $arg_idx = $arg_idx + 1
    #end while
#end if

    return JS_EXCEPTION;
}
