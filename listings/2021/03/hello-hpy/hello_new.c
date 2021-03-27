#include <hpy.h>

HPyDef_METH(add, "add", add_impl, HPyFunc_VARARGS,
            .doc = "add two integers");

static HPy add_impl(HPyContext ctx, HPy self, HPy *args, HPy_ssize_t nargs) {
    long a, b;
    if (!HPyArg_Parse(ctx, NULL, args, nargs, "ll", &a, &b))
        return HPy_NULL;
    return HPyLong_FromLong(ctx, a+b);
}

static HPyDef *hello_defines[] = {
    &add,
    NULL
};

static HPyModuleDef moduledef = {
    HPyModuleDef_HEAD_INIT,
    .m_name = "hello_new",
    .m_doc = "hello example using the new HPy API",
    .m_size = -1,
    .defines = hello_defines,
};

HPy_MODINIT(hello_new)
static HPy init_hello_new_impl(HPyContext ctx) {
    return HPyModule_Create(ctx, &moduledef);
}
