% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/combinations.R
\docType{data}
\name{Combinations}
\alias{Combinations}
\alias{icombinations}
\title{Combinations iterator}
\format{An object of class \code{R6ClassGenerator} of length 25.}
\usage{
Combinations

icombinations(x = NULL, k = NULL, n = NULL, v = NULL,
  freq = NULL, replace = FALSE, skip = NULL)
}
\arguments{
\item{x}{an integer or a vector, will be treated as \code{n} if integer; otherwise, will be treated as \code{v}.
Should not be specified together with \code{n} and \code{v}.}

\item{k}{an integer, the number of items drawn, defaults to \code{n} if \code{freq} is \code{NULL} else \code{sum(freq)}}

\item{n}{an integer, the total number of items, its value may be implicitly deduced from \code{length(v)} or \code{length(freq)}}

\item{v}{a vector to be drawn, defaults to \code{1:n}.}

\item{freq}{an integer vector of item repeat frequencies}

\item{replace}{an logical to draw items with replacement}

\item{skip}{the number of combinations skipped}
}
\description{
This function returns a \link{Combinations} iterator for iterating
combinations of \code{k} items from \code{n} items. The iterator allows users to fetch the next
combination(s) via the \code{getnext()} method.
}
\details{
The \code{Combinations} class can be initialized by using the convenient wrapper \code{icombinations} or
\preformatted{
Combinations$new(n, k, v = NULL, freq = NULL, replace = FALSE)
}

\preformatted{
getnext(d = 1L, layout = NULL, drop = NULL)
collect(layout = "row")
reset()
}
\describe{
\item{d}{number of fetched arrangements}

\item{layout}{if "row", "column" or "list" is specified, the returned value would be a
"row-major" matrix, a "column-major" matrix or a list respectively}

\item{drop}{vectorize a matrix or unlist a list}
}
}
\examples{
icomb <- icombinations(5, 2)
icomb$getnext()
icomb$getnext(2)
icomb$getnext(layout = "column", drop = FALSE)
# collect remaining combinations
icomb$collect()

library(foreach)
foreach(x = icombinations(5, 2), .combine=c) \%do\% {
  sum(x)
}
}
\seealso{
\link{combinations} for generating all combinations and \link{ncombinations} to calculate number of combinations
}
\keyword{datasets}
