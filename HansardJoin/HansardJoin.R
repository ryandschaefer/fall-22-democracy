require(devtools)
install_github("stephbuon/hansardr")
library(hansardr)
library(tidyverse)

hansard_join = function(decades = c(1800, 1810, 1820, 1830, 1840, 1850, 1860, 1870, 1880, 1890, 1900), files = c("hansard", "debate_metadata", "speaker_metadata", "file_metadata")) {
    for (d in decades) {
        print(d)
        for (f in files) {
            fn = paste(f, toString(d), sep = "_")
            cmd = paste("data('", fn, "')", sep = "")
            eval(parse(text = cmd))
            curr = get(fn)
            if (f == files[1]) {
                year = curr
            } else {
                year = inner_join(year, curr, by = "sentence_id")
            }
        }
        if (d == decades[1]) {
            df = year
        } else {
            df = rbind(df, year)
        }
    }
    return(df)
}

test = hansard_join(decades = c(1870, 1880))
test
