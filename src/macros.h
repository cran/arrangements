#ifndef _MACROS_H_
#define _MACROS_H_ 1

#define VALIDATE_ARGUMENTS() \
    if (Rf_isNull(_x)) { \
        n = Rf_isNull(_n) ? 0 : as_uint(_n); \
    } else { \
        if (Rf_length(_x) == 1 && Rf_isNumeric(_x)) { \
            n = as_uint(_x); \
        } else { \
            if (has_labels) { \
                Rf_error("confusing argument v"); \
            } else { \
                _v = _x; \
                has_labels = 1; \
            } \
            n = Rf_length(_v); \
        } \
    } \
    if (multiset) { \
        fp = as_uint_array(_freq); \
        flen = Rf_length(_freq); \
    } \
    if (has_labels) { \
        if (Rf_isNull(_freq)) { \
            n = Rf_length(_v); \
        } else if (replace) { \
            n = flen; \
            if (Rf_length(_v) != flen) Rf_error("length(v) != length(freq)"); \
        } else { \
            n = 0; \
            for (i = 0; i < Rf_length(_freq); i++) { \
                n += fp[i]; \
            } \
            if (Rf_length(_v) != flen) Rf_error("length(v) != s(freq)"); \
        } \
    } else { \
        if (Rf_isNull(_freq)) { \
            if (Rf_isNull(_x) && Rf_isNull(_n)) { \
                Rf_error("n is missing"); \
            } \
        } else if (replace) { \
            n = flen; \
        } else { \
            n = 0; \
            for (i = 0; i < flen; i++) { \
                n += fp[i]; \
            } \
        } \
    } \
    k = Rf_isNull(_k) ? n : as_uint(_k);


#define RESULT_PART() \
    if (layout == 'r') { \
        result = PROTECT(Rf_allocMatrix(INTSXP, d, n)); \
        nprotect++; \
        int* resultp = INTEGER(result); \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                resultp[j + i*d] = ap[i]; \
            } \
            for (i=k; i<n; i++) { \
                resultp[j + i*d] = 0; \
            } \
        } \
    } else if (layout == 'c') { \
        result = PROTECT(Rf_allocMatrix(INTSXP, n, d)); \
        nprotect++; \
        int * resultp = INTEGER(result); \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                resultp[j * n + i] = ap[i]; \
            } \
            for (i=k; i<n; i++) { \
                resultp[j * n + i] = 0; \
            } \
        } \
    } else if (layout == 'l') { \
        result = PROTECT(Rf_allocVector(VECSXP, d)); \
        nprotect++; \
        SEXP resulti; \
        int* resultp; \
        for (j=0; j<d; j++) { \
            NEXT(); \
            resulti = Rf_allocVector(INTSXP, k); \
            resultp = INTEGER(resulti); \
            for (i=0; i<k; i++) { \
                resultp[i] = ap[i]; \
            } \
            SET_VECTOR_ELT(result, j, resulti); \
        } \
    }

#define RESULT_K_PART() \
    if (layout == 'r') { \
        result = PROTECT(Rf_allocMatrix(INTSXP, d, k)); \
        nprotect++; \
        int* resultp = INTEGER(result); \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                resultp[j + i*d] = ap[i]; \
            } \
        } \
    } else if (layout == 'c') { \
        result = PROTECT(Rf_allocMatrix(INTSXP, k, d)); \
        nprotect++; \
        int * resultp = INTEGER(result); \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                resultp[j * k + i] = ap[i]; \
            } \
        } \
    } else if (layout == 'l') { \
        result = PROTECT(Rf_allocVector(VECSXP, d)); \
        nprotect++; \
        SEXP resulti; \
        int* resultp; \
        for (j=0; j<d; j++) { \
            NEXT(); \
            resulti = Rf_allocVector(INTSXP, k); \
            resultp = INTEGER(resulti); \
            for (i=0; i<k; i++) { \
                resultp[i] = ap[i]; \
            } \
            SET_VECTOR_ELT(result, j, resulti); \
        } \
    }


#define RESULT_NILSXP(k) \
    if (layout == 'r') { \
        result = PROTECT(Rf_allocMatrix(INTSXP, d, k)); \
        nprotect++; \
        int* resultp = INTEGER(result); \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                resultp[j + i*d] = ap[i] + 1; \
            } \
        } \
    } else if (layout == 'c') { \
        result = PROTECT(Rf_allocMatrix(INTSXP, k, d)); \
        nprotect++; \
        int * resultp = INTEGER(result); \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                resultp[j * k + i] = ap[i] + 1; \
            } \
        } \
    } else if (layout == 'l') { \
        result = PROTECT(Rf_allocVector(VECSXP, d)); \
        nprotect++; \
        SEXP resulti; \
        int* resultp; \
        for (j=0; j<d; j++) { \
            NEXT(); \
            resulti = Rf_allocVector(INTSXP, k); \
            resultp = INTEGER(resulti); \
            for (i=0; i<k; i++) { \
                resultp[i] = ap[i] + 1; \
            } \
            SET_VECTOR_ELT(result, j, resulti); \
        } \
    }

#define RESULT_INTSXP(k) \
    int* labelsp = INTEGER(labels); \
    if (layout == 'r') { \
        result = PROTECT(Rf_allocMatrix(INTSXP, d, k)); \
        nprotect++; \
        int* resultp = INTEGER(result); \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                resultp[j + i*d] = labelsp[ap[i]]; \
            } \
        } \
    } else if (layout == 'c') { \
        result = PROTECT(Rf_allocMatrix(INTSXP, k, d)); \
        nprotect++; \
        int * resultp = INTEGER(result); \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                resultp[j * k + i] = labelsp[ap[i]]; \
            } \
        } \
    } else if (layout == 'l') { \
        result = PROTECT(Rf_allocVector(VECSXP, d)); \
        nprotect++; \
        SEXP resulti; \
        int* resultp; \
        for (j=0; j<d; j++) { \
            NEXT(); \
            resulti = Rf_allocVector(INTSXP, k); \
            resultp = INTEGER(resulti); \
            for (i=0; i<k; i++) { \
                resultp[i] = labelsp[ap[i]]; \
            } \
            SET_VECTOR_ELT(result, j, resulti); \
        } \
    }

#define RESULT_REALSXP(k) \
    double* labelsp = REAL(labels); \
    if (layout == 'r') { \
        result = PROTECT(Rf_allocMatrix(REALSXP, d, k)); \
        nprotect++; \
        double* resultp = REAL(result); \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                resultp[j + i*d] = labelsp[ap[i]]; \
            } \
        } \
    } else if (layout == 'c') { \
        result = PROTECT(Rf_allocMatrix(REALSXP, k, d)); \
        nprotect++; \
        double * resultp = REAL(result); \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                resultp[j * k + i] = labelsp[ap[i]]; \
            } \
        } \
    } else if (layout == 'l') { \
        result = PROTECT(Rf_allocVector(VECSXP, d)); \
        nprotect++; \
        SEXP resulti; \
        double* resultp; \
        for (j=0; j<d; j++) { \
            NEXT(); \
            resulti = Rf_allocVector(REALSXP, k); \
            resultp = REAL(resulti); \
            for (i=0; i<k; i++) { \
                resultp[i] = labelsp[ap[i]]; \
            } \
            SET_VECTOR_ELT(result, j, resulti); \
        } \
    }

#define RESULT_STRSXP(k) \
    if (layout == 'r') { \
        result = PROTECT(Rf_allocMatrix(STRSXP, d, k)); \
        nprotect++; \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                SET_STRING_ELT(result, j + i*d, STRING_ELT(labels, ap[i])); \
            } \
        } \
    } else if (layout == 'c') { \
        result = PROTECT(Rf_allocMatrix(STRSXP, k, d)); \
        nprotect++; \
        for (j=0; j<d; j++) { \
            NEXT(); \
            for (i=0; i<k; i++) { \
                SET_STRING_ELT(result, j * k + i, STRING_ELT(labels, ap[i])); \
            } \
        } \
    } else if (layout == 'l') { \
        result = PROTECT(Rf_allocVector(VECSXP, d)); \
        nprotect++; \
        SEXP resulti; \
        for (j=0; j<d; j++) { \
            NEXT(); \
            resulti = Rf_allocVector(STRSXP, k); \
            for (i=0; i<k; i++) { \
                SET_STRING_ELT(resulti, i, STRING_ELT(labels, ap[i])); \
            } \
            SET_VECTOR_ELT(result, j, resulti); \
        } \
    }

#endif