#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <gmp.h>
#include "arrangements.h"
#include "next/multiset_combination.h"
#include "utils.h"

double ncombinations_f(int* freq, size_t flen, size_t k);

SEXP next_multiset_combinations(SEXP _n, SEXP _k, SEXP _d, SEXP state, SEXP labels, SEXP freq, SEXP _type) {
    size_t i, j, h;

    size_t n = as_uint(_n);
    size_t k = as_uint(_k);
    int d;
    double dd;
    if (Rf_asInteger(_d) == -1) {
        dd = ncombinations_f(INTEGER(freq), Rf_length(freq), k);
    } else {
        dd = as_uint(_d);
    }

    int ltype = TYPEOF(labels);
    int* labels_intp;
    double* labels_doublep;

    int* fp;

    char type;
    if (_type == R_NilValue) {
        type = 'r';
    } else {
        type = CHAR(Rf_asChar(_type))[0];
        if (type != 'r' && type != 'c' && type != 'l') type = 'r';
    }

    if (type == 'l') {
        if (dd > INT_MAX) Rf_error("too many results");
    } else {
        if (dd * k > INT_MAX) Rf_error("too many results");
    }
    d = round(dd);

    SEXP ms;
    unsigned int* mp;
    SEXP as;
    unsigned int* ap;
    int nprotect = 0;

    int status = 0;

    if (state == R_NilValue) {
        ms = R_UnboundValue;
        as = R_UnboundValue;
    } else {
        ms = PROTECT(Rf_findVarInFrame(state, Rf_install("m")));
        as = PROTECT(Rf_findVarInFrame(state, Rf_install("a")));
        nprotect += 2;
    }

    if (ms == R_UnboundValue) {
        if (state == R_NilValue) {
            mp = (unsigned int*) R_alloc(n, sizeof(int));
        } else {
            ms = PROTECT(Rf_allocVector(INTSXP, n));
            Rf_defineVar(Rf_install("m"), ms, state);
            UNPROTECT(1);
            mp = (unsigned int*) INTEGER(ms);
        }
        fp = INTEGER(freq);
        h = 0;
        for (i = 0; i< Rf_length(freq); i++) {
            for (j = 0; j< fp[i]; j++) {
                mp[h++] = i;
            }
        }

    } else {
        mp = (unsigned int*) INTEGER(ms);
    }

    if (as == R_UnboundValue) {
        if (state == R_NilValue) {
            ap = (unsigned int*) R_alloc(k, sizeof(int));
        } else {
            as = PROTECT(Rf_allocVector(INTSXP, k));
            Rf_defineVar(Rf_install("a"), as, state);
            UNPROTECT(1);
            ap = (unsigned int*) INTEGER(as);
        }
        fp = INTEGER(freq);
        h = 0;
        for (i = 0; i< k; i++) {
            ap[i] = mp[i];
        }

    } else {
        ap = (unsigned int*) INTEGER(as);
        status = 1;
    }

    SEXP rdim;
    SEXP result, resulti;
    int* result_intp;
    double* result_doublep;

    if (type == 'r') {
        if (labels == R_NilValue) {
            result = PROTECT(Rf_allocVector(INTSXP, k*d));
            nprotect++;
            result_intp = INTEGER(result);
        } else {
            result = PROTECT(Rf_allocVector(ltype, k*d));
            nprotect++;
            if (ltype == INTSXP) {
                result_intp = INTEGER(result);
                labels_intp = INTEGER(labels);
            } else if (ltype == REALSXP) {
                result_doublep = REAL(result);
                labels_doublep = REAL(labels);
            }
        }

        for (j=0; j<d; j++) {
            if (status) {
                if (!next_multiset_combination(mp, ap, n, k)) {
                    status = 0;
                    break;
                }
            } else {
                status = 1;
            }
            if (ltype == NILSXP) {
                for (i=0; i<k; i++) {
                    result_intp[j + i*d] = ap[i] + 1;
                }
            } else if (ltype == INTSXP) {
                for (i=0; i<k; i++) {
                    result_intp[j + i*d] = labels_intp[ap[i]];
                }
            } else if (ltype == REALSXP) {
                for (i=0; i<k; i++) {
                    result_doublep[j + i*d] = labels_doublep[ap[i]];
                }
            } else if (ltype == STRSXP) {
                for (i=0; i<k; i++) {
                    SET_STRING_ELT(result, j + i*d, STRING_ELT(labels, ap[i]));
                }
            }
        }
        if (status == 0) {
            result = PROTECT(resize_row(result, k, d, j));
            nprotect++;
        }
        PROTECT(rdim = Rf_allocVector(INTSXP, 2));
        INTEGER(rdim)[0] = j;
        INTEGER(rdim)[1] = k;
        Rf_setAttrib(result, R_DimSymbol, rdim);
        UNPROTECT(1);

    } else if (type == 'c') {
        if (labels == R_NilValue) {
            result = PROTECT(Rf_allocVector(INTSXP, k*d));
            nprotect++;
            result_intp = INTEGER(result);
        } else {
            result = PROTECT(Rf_allocVector(ltype, k*d));
            nprotect++;
            if (ltype == INTSXP) {
                result_intp = INTEGER(result);
                labels_intp = INTEGER(labels);
            } else if (ltype == REALSXP) {
                result_doublep = REAL(result);
                labels_doublep = REAL(labels);
            }
        }

        for (j=0; j<d; j++) {
            if (status) {
                if (!next_multiset_combination(mp, ap, n, k)) {
                    status = 0;
                    break;
                }
            } else {
                status = 1;
            }
            if (ltype == NILSXP) {
                for (i=0; i<k; i++) {
                    result_intp[j * k + i] = ap[i] + 1;
                }
            } else if (ltype == INTSXP) {
                for (i=0; i<k; i++) {
                    result_intp[j * k + i] = labels_intp[ap[i]];
                }
            } else if (ltype == REALSXP) {
                for (i=0; i<k; i++) {
                    result_doublep[j * k + i] = labels_doublep[ap[i]];
                }
            } else if (ltype == STRSXP) {
                for (i=0; i<k; i++) {
                    SET_STRING_ELT(result, j * k + i, STRING_ELT(labels, ap[i]));
                }
            }
        }
        if (status == 0) {
            result = PROTECT(resize_col(result, k, d, j));
            nprotect++;
        }
        PROTECT(rdim = Rf_allocVector(INTSXP, 2));
        INTEGER(rdim)[0] = k;
        INTEGER(rdim)[1] = j;
        Rf_setAttrib(result, R_DimSymbol, rdim);
        UNPROTECT(1);

    } else {
        // type == "list"
        result = PROTECT(Rf_allocVector(VECSXP, d));
        nprotect++;
        if (ltype == INTSXP) {
            labels_intp = INTEGER(labels);
        } else if (ltype == REALSXP) {
            labels_doublep = REAL(labels);
        }

        for (j=0; j<d; j++) {
            if (status) {
                if (!next_multiset_combination(mp, ap, n, k)) {
                    status = 0;
                    break;
                }
            } else {
                status = 1;
            }
            if (ltype == NILSXP) {
                resulti = Rf_allocVector(INTSXP, k);
                result_intp = INTEGER(resulti);
                for (i=0; i<k; i++) {
                    result_intp[i] = ap[i] + 1;
                }
            } else if (ltype == INTSXP) {
                resulti = Rf_allocVector(INTSXP, k);
                result_intp = INTEGER(resulti);
                for (i=0; i<k; i++) {
                    result_intp[i] = labels_intp[ap[i]];
                }
            } else if (ltype == REALSXP) {
                resulti = Rf_allocVector(REALSXP, k);
                result_doublep = REAL(resulti);
                for (i=0; i<k; i++) {
                    result_doublep[i] = labels_doublep[ap[i]];
                }
            } else if (ltype == STRSXP) {
                resulti = Rf_allocVector(STRSXP, k);
                for (i=0; i<k; i++) {
                    SET_STRING_ELT(resulti, i, STRING_ELT(labels, ap[i]));
                }
            }
            SET_VECTOR_ELT(result, j, resulti);
        }
        if (status == 0) {
            result = PROTECT(resize_list(result, d, j));
            nprotect++;
        }
    }

    UNPROTECT(nprotect);
    return result;
}


double ncombinations_f(int* freq, size_t flen, size_t k) {
    int n = 0;
    int i, j, h;
    for (i=0; i<flen; i++) n += freq[i];
    if (k > n) {
        return 0;
    }

    double* p = (double*) malloc((k+1) * sizeof(double));
    for (j=0; j<=k; j++) p[j] = 0;

    double ptemp;

    for (i=0; i<flen; i++) {
        if (i == 0) {
            for (j=0; j<=k && j<=freq[i]; j++) {
                p[j] = 1;
            }
            ptemp = p[k];
        } else if (i < flen - 1){
            for (j=k; j>0; j--) {
                ptemp = 0;
                for(h=0; h<=freq[i] && h<=j; h++) {
                    ptemp += p[j-h];
                }
                p[j] = ptemp;
            }
        } else {
            ptemp = 0;
            for(h=0; h<=freq[i] && h<=k; h++) {
                ptemp += p[k-h];
            }
        }
    }

    free(p);
    return ptemp;
}

SEXP ncomb_f(SEXP freq, SEXP _k) {
    int* fp = INTEGER(freq);
    size_t flen = Rf_length(freq);
    size_t k = as_uint(_k);
    return Rf_ScalarReal(ncombinations_f(fp, flen, k));
}

void ncombinations_f_bigz(mpz_t z, int* freq, size_t flen, size_t k) {
    int n = 0;
    int i, j, h;
    for (i=0; i<flen; i++) n += freq[i];
    if (k > n) {
        mpz_set_ui(z, 0);
        return;
    }

    mpz_t* p = (mpz_t*) malloc((k+1) * sizeof(mpz_t));
    for (j=0; j<=k; j++) mpz_init(p[j]);

    for (i=0; i<flen; i++) {
        if (i == 0) {
            for (j=0; j<=k && j<=freq[i]; j++) {
                mpz_set_ui(p[j], 1);
            }
            mpz_set(z, p[k]);
        } else if (i < flen - 1){
            for (j=k; j>0; j--) {
                mpz_set_ui(z, 0);
                for(h=0; h<=freq[i] && h<=j; h++) {
                    mpz_add(z, z, p[j-h]);
                }
                mpz_set(p[j], z);
            }
        } else {
            mpz_set_ui(z, 0);
            for(h=0; h<=freq[i] && h<=k; h++) {
                mpz_add(z, z, p[k-h]);
            }
        }
    }
}

SEXP ncomb_f_bigz(SEXP freq, SEXP _k) {
    int* fp = INTEGER(freq);
    size_t flen = Rf_length(freq);
    size_t k = as_uint(_k);
    mpz_t z;
    mpz_init(z);
    ncombinations_f_bigz(z, fp, flen, k);
    char* c = mpz_get_str(NULL, 10, z);
    SEXP out = Rf_mkString(c);
    mpz_clear(z);
    free(c);
    return out;
}
