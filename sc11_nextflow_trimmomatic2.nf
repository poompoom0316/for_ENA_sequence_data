#!/usr/bin/env nextflow

nextflow.enable.dsl=2


/*  Comments are uninterpreted text included with the script.
    They are useful for describing complex parts of the workflow
    or providing useful information such as workflow usage.

    Usage:
    nextflow run wc.nf --input <input_file>

    Multi-line comments start with a slash asterisk /* and finish with an asterisk slash. */
//  Single line comments start with a double slash // and finish on the same line

/*  Workflow parameters are written as params.<parameter>
    and can be initialised using the `=` operator. */

params.inputdir = "data"
params.output_dir = "analysis/sc10"
params.sampleid = "SRR21977427"
params.output = "analysis/sc10/SRR21977427_trimmed_2.fastq.gz"



workflow {
    //  Input data is received through channels
    input_dir = "${params.inputdir}/${params.sampleid}.fastq.gz".toString()
    output_dir = "${params.output_dir}/${params.sampleid}_trimmed_2.fastq.gz".toString()
    input_ch = Channel.fromPath(input_dir)
    output_path = Channel.fromPath(output_dir)
    
    
    /*  The script to execute is called by its process name,
    and input is provided between brackets. */
    TRIMMOMATIC(input_ch, output_path)

    /*  Process output is accessed using the `out` channel.
    The channel operator view() is used to print
    process output to the terminal. */
    TRIMMOMATIC.out.view({ "Received: $it" })

}


/*  A Nextflow process block
    Process names are written, by convention, in uppercase.
    This convention is used to enhance workflow readability. */
process TRIMMOMATIC {
    input:
    path read
    path output_path

    output:
    stdout

    script:
    /* Triple quote syntax """, Triple-single-quoted strings may span multiple lines. The content of the string can cross line boundaries without the need to split the string in several pieces and without concatenation or newline escape characters. */
    
    """
    
    printf '${read}\n'
    trimmomatic SE    -threads 4             \
    -phred33         \
    ${read} ${output_path}   LEADING:20 \
    TRAILING:20 \
    SLIDINGWINDOW:4:15 \
    MINLEN:36
    """

}

