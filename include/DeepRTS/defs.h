
// XTENSOR does not compile properly because of pow10 errors. This "fixes it"
#ifdef __APPLE__
    #undef _GNU_SOURCE
#endif
