% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/partitions.R
\name{partitions}
\alias{partitions}
\title{Partitions generator}
\usage{
partitions(n, k = NULL, distinct = FALSE, descending = FALSE,
  layout = NULL, nitem = -1L, skip = NULL, index = NULL,
  nsample = NULL, drop = NULL)
}
\arguments{
\item{n}{an non-negative integer to be partitioned}

\item{k}{number of parts}

\item{distinct}{an logical to restrict distinct values}

\item{descending}{an logical to use reversed lexicographical order}

\item{layout}{if "row", "column" or "list" is specified, the returned value would be a
"row-major" matrix, a "column-major" matrix or a list respectively}

\item{nitem}{number of partitions required, usually used with \code{skip}}

\item{skip}{the number of partitions skipped}

\item{index}{a vector of indices of the desired partitions}

\item{nsample}{sampling random partitions}

\item{drop}{vectorize a matrix or unlist a list}
}
\description{
This function partitions an non-negative interger \code{n} into \code{k} parts or parts of any sizes.
The results are in lexicographical or reversed lexicographical order.
}
\examples{
# all partitions of 6
partitions(6)
# reversed lexicographical order
partitions(6, descending = TRUE)

# fixed number of parts
partitions(10, 5)
# reversed lexicographical order
partitions(10, 5, descending = TRUE)

# column major
partitions(6, layout = "column")
partitions(6, 3, layout = "column")

# list output
partitions(6, layout = "list")
partitions(6, 3, layout = "list")

# zero sized partitions
dim(partitions(0))
dim(partitions(5, 0))
dim(partitions(5, 6))
dim(partitions(0, 0))
dim(partitions(0, 1))

}
\seealso{
\link{ipartitions} for iterating partitions and \link{npartitions} to calculate number of partitions
}
