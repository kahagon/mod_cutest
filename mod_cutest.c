/* 
**  mod_cutest.c -- Apache sample cutest module
**  [Autogenerated via ``apxs -n cutest -g'']
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory 
**  by running:
**
**    $ apxs -c -i mod_cutest.c
**
**  Then activate it in Apache's apache2.conf file for instance
**  for the URL /cutest in as follows:
**
**    #   apache2.conf
**    LoadModule cutest_module modules/mod_cutest.so
**    <Location /cutest>
**    SetHandler cutest
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
**
**  you immediately can request the URL /cutest and watch for the
**  output of this module. This can be achieved for instance via:
**
**    $ lynx -mime_header http://localhost/cutest 
**
**  The output should be similar to the following one:
**
**    HTTP/1.1 200 OK
**    Date: Tue, 31 Mar 1998 14:42:22 GMT
**    Server: Apache/1.3.4 (Unix)
**    Connection: close
**    Content-Type: text/html
**  
**    The sample page from mod_cutest.c
*/ 

#include <dlfcn.h>
#include <limits.h>
#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"
#include "apreq_param.h"
#include "ModCuTest.h"

static char *cutest_get_target_dl_path(request_rec *r) {
    if (!r->args) return NULL;
    
    apr_table_t *query_table = apr_table_make(r->pool, 1);
    apr_status_t result = apreq_parse_query_string(r->pool, query_table, r->args);
    if (result != APR_SUCCESS || apr_is_empty_table(query_table)) {
        return NULL;
    }
    
    return apr_table_get(query_table, "dl_path");
}

typedef CuSuite* (*_CuGetSuite)();

static void cutest_run_all_test(request_rec *r, const char *dl_path) {
    if (!dl_path) {
        ap_rputs("specify query dl_path.", r);
        return;
    }
    char *dl_real_path = apr_pcalloc(r->pool, PATH_MAX);
    if (!realpath(dl_path, dl_real_path)) {
        ap_rputs("specify query dl_path.", r);
        return;
    }
    void *dlh = dlopen(dl_path, RTLD_NOW | RTLD_LOCAL);
    if (!dlh) {
        ap_rputs(dlerror(), r);
        return;
    }
    _CuGetSuite f = (_CuGetSuite)dlsym(dlh, "CuGetSuite");
    if (!f) {
        ap_rputs(dlerror(), r);
        return;
    }
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();
    CuSuiteAddSuite(suite, (*f)());

    CuSuiteRun(suite, r);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    ap_rwrite(output->buffer, output->length, r);
    if (dlclose(dlh) != 0) {
        ap_rputs(dlerror(), r);
        return;
    }
}

/* The sample content handler */
static int cutest_handler(request_rec *r)
{
    if (strcmp(r->handler, "cutest")) {
        return DECLINED;
    }
    r->content_type = "text/plain";      

    if (r->header_only)
        return OK;
    
    char *dl_path = cutest_get_target_dl_path(r);
    cutest_run_all_test(r, dl_path);
    
    return OK;
}

static void cutest_register_hooks(apr_pool_t *p)
{
    ap_hook_handler(cutest_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA cutest_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    cutest_register_hooks  /* register hooks                      */
};

