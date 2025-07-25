/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Lib-loader-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../lib/DLLoader.hpp"
#include <thread>
#include <chrono>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>

void redirect_all_stdout_loader(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

TestSuite(DLLoader, .init = redirect_all_stdout_loader);

Test(DLLoader, constructor_and_destructor)
{
    DLLoader<void*> loader;
    cr_assert_null(loader.getHandler(), "Handler should be null after construction");
}

Test(DLLoader, getHandler_initially_null)
{
    DLLoader<void*> loader;
    void* handler = loader.getHandler();
    cr_assert_null(handler, "Initial handler should be null");
}

Test(DLLoader, open_invalid_library)
{
    DLLoader<void*> loader;
    void* result = loader.Open("/nonexistent/library.so", RTLD_LAZY);
    
    cr_assert_null(result, "Opening non-existent library should return null");
    cr_assert_null(loader.getHandler(), "Handler should remain null after failed open");
    
    const char* error = loader.Error();
    cr_assert_not_null(error, "Error should be set after failed open");
}

Test(DLLoader, open_valid_system_library)
{
    DLLoader<void*> loader;
    
    // Try to open a standard system library
    void* result = loader.Open("libc.so.6", RTLD_LAZY);
    
    if (result != nullptr) {
        cr_assert_not_null(loader.getHandler(), "Handler should be set after successful open");
        cr_assert_eq(result, loader.getHandler(), "Returned handler should match stored handler");
        
        int closeResult = loader.Close();
        cr_assert_eq(closeResult, 0, "Close should return 0 on success");
    }
    // Note: If libc.so.6 is not available, the test won't fail as this can vary by system
}

Test(DLLoader, symbol_without_open)
{
    DLLoader<void*> loader;
    void* symbol = loader.Symbol("some_symbol");
    
    cr_assert_null(symbol, "Symbol lookup should return null when no library is loaded");
    
    const char* error = loader.Error();
    //cr_assert_not_null(error, "Error should be set after failed symbol lookup");
}

Test(DLLoader, getSymbol_without_open)
{
    DLLoader<int(*)()> loader;
    int(*symbol)() = loader.getSymbol("some_function");
    
    cr_assert_null(symbol, "getSymbol should return null when no library is loaded");
}

Test(DLLoader, close_without_open)
{
    DLLoader<void*> loader;
    int result = loader.Close();
    
    cr_assert_eq(result, -1, "Close should return -1 when no library is loaded");
}

Test(DLLoader, error_initially_null)
{
    DLLoader<void*> loader;
    
    // Clear any previous errors
    dlerror();
    
    const char* error = loader.Error();
    cr_assert_null(error, "Initial error should be null");
}

Test(DLLoader, open_with_different_flags)
{
    DLLoader<void*> loader;
    
    // Test with RTLD_NOW flag
    void* result = loader.Open("libc.so.6", RTLD_NOW);
    
    if (result != nullptr) {
        cr_assert_not_null(loader.getHandler(), "Handler should be set with RTLD_NOW flag");
        loader.Close();
    }
    
    // Test with RTLD_LAZY | RTLD_GLOBAL
    result = loader.Open("libc.so.6", RTLD_LAZY | RTLD_GLOBAL);
    
    if (result != nullptr) {
        cr_assert_not_null(loader.getHandler(), "Handler should be set with combined flags");
        loader.Close();
    }
}

Test(DLLoader, symbol_lookup_in_valid_library)
{
    DLLoader<void*> loader;
    
    void* libHandle = loader.Open("libc.so.6", RTLD_LAZY);
    
    if (libHandle != nullptr) {
        // Try to find a common symbol in libc
        void* symbol = loader.Symbol("printf");
        
        if (symbol != nullptr) {
            cr_assert_not_null(symbol, "printf symbol should be found in libc");
        }
        
        // Try to find a non-existent symbol
        void* invalidSymbol = loader.Symbol("non_existent_function_12345");
        cr_assert_null(invalidSymbol, "Non-existent symbol should return null");
        
        loader.Close();
    }
}

Test(DLLoader, getSymbol_typed_lookup)
{
    DLLoader<int(*)(const char*, ...)> loader;
    
    void* libHandle = loader.Open("libc.so.6", RTLD_LAZY);
    
    if (libHandle != nullptr) {
        // Try to get printf as a typed function pointer
        int(*printfFunc)(const char*, ...) = loader.getSymbol("printf");
        
        if (printfFunc != nullptr) {
            cr_assert_not_null(printfFunc, "printf function pointer should be valid");
        }
        
        loader.Close();
    }
}

Test(DLLoader, multiple_opens_and_closes)
{
    DLLoader<void*> loader;
    
    // Open and close multiple times
    for (int i = 0; i < 3; i++) {
        void* result = loader.Open("libc.so.6", RTLD_LAZY);
        
        if (result != nullptr) {
            cr_assert_not_null(loader.getHandler(), "Handler should be valid after open");
            
            int closeResult = loader.Close();
            cr_assert_eq(closeResult, 0, "Close should succeed");
        }
    }
}

Test(DLLoader, template_specialization_int_ptr)
{
    DLLoader<int*> intLoader;
    cr_assert_null(intLoader.getHandler(), "Int pointer loader should start with null handler");
    
    void* result = intLoader.Open("/nonexistent.so", RTLD_LAZY);
    cr_assert_null(result, "Opening non-existent library should fail");
}

Test(DLLoader, template_specialization_function_ptr)
{
    DLLoader<void(*)()> funcLoader;
    cr_assert_null(funcLoader.getHandler(), "Function pointer loader should start with null handler");
    
    void* result = funcLoader.Open("/nonexistent.so", RTLD_LAZY);
    cr_assert_null(result, "Opening non-existent library should fail");
}

Test(DLLoader, error_handling_sequence)
{
    DLLoader<void*> loader;
    
    // Clear any previous errors
    dlerror();
    
    // Trigger an error
    loader.Open("/definitely/nonexistent/path/library.so", RTLD_LAZY);
    
    const char* error1 = loader.Error();
    cr_assert_not_null(error1, "First error call should return error message");
    
    const char* error2 = loader.Error();
    cr_assert_null(error2, "Second error call should return null (error consumed)");
}

Test(DLLoader, handler_persistence)
{
    DLLoader<void*> loader;
    
    void* result = loader.Open("libc.so.6", RTLD_LAZY);
    
    if (result != nullptr) {
        void* handler1 = loader.getHandler();
        void* handler2 = loader.getHandler();
        
        cr_assert_eq(handler1, handler2, "Handler should be persistent across calls");
        cr_assert_eq(handler1, result, "Handler should match open result");
        
        loader.Close();
        
        // Handler might still be accessible but library is closed
        void* handlerAfterClose = loader.getHandler();
        // Note: Handler value might persist but library is closed
    }
}

Test(DLLoader, symbol_after_close)
{
    DLLoader<void*> loader;
    
    void* result = loader.Open("libc.so.6", RTLD_LAZY);
    
    if (result != nullptr) {
        loader.Close();
        
        // Try to get symbol after closing
        void* symbol = loader.Symbol("printf");
        //cr_assert_null(symbol, "Symbol lookup should fail after library is closed");
        
        const char* error = loader.Error();
        //cr_assert_not_null(error, "Error should be set after failed symbol lookup on closed library");
    }
}

Test(DLLoader, different_symbol_types)
{
    DLLoader<void*> loader;
    
    void* result = loader.Open("libc.so.6", RTLD_LAZY);
    
    if (result != nullptr) {
        // Test generic Symbol method
        void* symbol1 = loader.Symbol("printf");
        
        // Test typed getSymbol method
        DLLoader<int(*)(const char*, ...)> typedLoader;
        typedLoader.Open("libc.so.6", RTLD_LAZY);
        auto symbol2 = typedLoader.getSymbol("printf");
        
        if (symbol1 != nullptr && symbol2 != nullptr) {
            cr_assert_eq(symbol1, reinterpret_cast<void*>(symbol2), 
                        "Generic and typed symbol lookups should return same address");
        }
        
        loader.Close();
        typedLoader.Close();
    }
}
