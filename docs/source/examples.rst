Examples
=========

In this section, we show the process of creating AlgoRun containers for 3 different examples of published software: [#f1]_ the popular bioinformatics software Bowtie (Langmead, 2009), [#f2]_ REACT (Vera-Licona, 2014), a systems biology software to infer gene regulatory networks and, [#f3]_ the KS algorithm to solve the transversal hypergraph generation problem (Kavvadias, 2005). 
For the first example, Bowtie, we show how to create an AlgoRun container, how to run the Bowtie AlgoRun container using AlgoRun web interface, how to expose command line options as parameters, some input examples to highlight the use of command line options vs. parameters and finally, how to access the tool deployed with AlgoRun via a RESTful API interface. For the other two examples we show how to create the AlgoRun containers and provide the users with the appropriate links to allow users to deploy and use all the AlgoRun features as presented in the first example.
 
Packaging Bowtie Software with AlgoRun
--------------------------------------

Bowtie (Langmead, 2009) is an ultra-fast memory-efficient short read aligner. The source code is written in C++ and is available under the Artistic License. Download it from http://sourceforge.net/projects/bowtie-bio/files/bowtie/1.1.2/ 

Unzip the downloaded file. This unzipped file will contain all the source code of Bowtie.

**STEP 1**: Add all Bowtie source files inside the src folder.

**STEP 2**: Add the instructions to install the C++ dependencies as well as the instructions to build Bowtie source code to the Dockerfile. Below is how the Dockerfile of Bowtie looks like.

.. image:: /images/bowtie-docker.png
        :align: center
.. centered:: Source code: Dockerfile of Bowtie software

Hints:
#######
1. Dockerfile syntax requires preceding all commands with RUN keyword.
2. To ensure successful installation, always use apt-get update before installing packages and use -y option in the install command.
3. Change to /home/algorithm/src directory before running any command that operates on the source files inside src folder.


**STEP 3**: *manifest.json* file is required to describe the computational algorithm. Comments in the file will guide you to fill the correct values. Below is how the manifest of Bowtie looks like.

.. image:: /images/bowtie-manifest.png
        :align: center
.. centered:: Source Code: manifest.json of Bowtie software (comments-skimmed)

**STEP 4**:

* input_example.txt file includes a sample input data for users to quickly try the algorithm. Enter ATGCATCATGCGCCAT as an example.
* output_example.txt file includes a sample of the expected output for the same input. It makes it easier for users to expect the results. The above input produces the following:
    *0	-	gi|110640213|ref|NC_008253.1|	148810	ATGGCGCATGATGCAT	IIIIIIIIIIIIIIII	0	10:A>G,13:C>G*

Notes:
#######
* Bowtie source code comes with e_coli index packaged by default. So, use it in the algo_exec. If you included other indexes, it’s ok to use them as well.
* Use direct in algo_input_stream to accept input directly from the command line. Bowtie has other options to read the input from a file. However, AlgoRun will automatically present an option to upload a file to the input area in the web interface.
* Use stdout in algo_output_stream to let AlgoRun get the result from the terminal. Bowtie has other options to write the output to a file. However, AlgoRun will automatically present an option to download the result to a file from the web interface. 

**STEP 5**:

- From the directory where the Dockerfile exists, build Bowtie container using:
    `docker build -t bowtie .`
- You should see a success message as in the following picture.


.. image:: /images/bowtie-build.png
        :align: center
.. centered:: Bowtie container build success message

User Interface
###############

Run Bowtie container using:

.. code-block:: shell

    docker run -p 31331:8765 bowtie

Open the web browser and type http://localhost:31331 

**Hint**: You can use any available port other than 31331. Yet, you must bind it to 8765 port as it is the gateway to AlgoRun.

.. image:: /images/bowtie-web.png
        :align: center
        
[OPTIONAL] Expose Command Line Options as Parameters
#####################################################

To give flexibility to an implemented algorithm, AlgoRun allows exposing parameters that can be easily changed from the web interface. These parameters will be available as environment variables in the source code.  

The power of Bowtie as a very fast DNA sequences aligner comes from the available command line options. So, you can make use of AlgoRun parameters to expose these command line options. You have two options: either to manipulate the source code of Bowtie so that it reads options from environment variables (instead of command line) or to develop a wrapper around Bowtie main executable that will internally translate environment variables to command line options. To do so, follow the below steps:

.. image:: /images/bowtie-parameters.png
        :align: right
        :height: 400px
        
1.	Specify parameters and their default values in the manifest file. The adjacent picture shows some parameters.
2.	Read the input data. The input data is passed as the first command line argument.
3.	Read the environment variables (of the same names you specified in the manifest) and form the options string. Call the executable file and to print the output to the standard output.

Modify the Dockerfile to install ruby dependency:

.. code-block:: shell

    RUN apt-get update && apt-get install -y ruby build-essential

Modify algo_exec value in the manifest file to: 

.. code-block:: shell

    "algo_exec": "ruby bowtie.rb",
    
Rebuild Bowtie container using: 

.. code-block:: shell

    `docker build -t bowtie .`

At this point, options available from Bowtie can be changed by clicking on “Change Parameters” button from the web interface. Visit http://bowtie.algorun.org for the final version of Bowtie running inside AlgoRun standard container.
Find the complete example on AlgoRun GitHub repository (https://github.com/algorun/algorun/tree/master/examples/bowtie-1.1.2).

.. image:: /images/bowtie-wrapper.png
        :align: center
.. centered:: Source Code: bowtie.rb wrapper code


.. image:: /images/bowtie-web-interface.png
        :align: center
.. centered:: Bowtie web interface


Input Examples (Command Line Options vs. Parameters)
######################################################

1. Example Link: http://bowtie-bio.sourceforge.net/manual.shtml#example-1--a

    Command line: ./bowtie -a -v 2 e_coli --suppress 1,5,6,7 –c ATGCATCATGCGCCAT

    With AlgoRun: Change Report-all to on, Align-v to 2 and suppress to 1,5,6,7

2. Example Link: http://bowtie-bio.sourceforge.net/manual.shtml#example-2--k-3

    Command line: ./bowtie -k 3 -v 2 e_coli --suppress 1,5,6,7 –c ATGCATCATGCGCCAT

    With AlgoRun: Change Report-k to 3, Align-v to 2 and suppress to 1,5,6,7

3. Example Link: http://bowtie-bio.sourceforge.net/manual.shtml#example-3--k-6

    Command line: ./bowtie -k 6 -v 2 e_coli --suppress 1,5,6,7 -c ATGCATCATGCGCCAT

    With AlgoRun: Change Report-k to 6, Align-v to 2 and suppress to 1,5,6,7

4. Example Link: http://bowtie-bio.sourceforge.net/manual.shtml#example-9--a--m-3---best---strata

    Command line: ./bowtie -a -m 3 --best --strata -v 2 e_coli --suppress 1,5,6,7 -c ATGCATCATGCGCCAT

    With AlgoRun: Change Report-all to on, Report-m to 3, Report-best to on, Report-strata to on, Align-v to 2 and suppress to 1,5,6,7.


Running Bowtie through AlgoRun’s Web API
###########################################

In addition to the web user interface available at http://bowtie.algorun.org, you can run Bowtie using the web API. Using the web API is useful to perform the computation from different client applications. As an example of running Bowtie through the web API, see the Firefox Poster plugin examples below. Refer to supplementary file 1 for a detailed illustration on web APIs.

.. image:: /images/bowtie-api-call.png
        :align: center
.. centered:: Run Bowtie Computation: (1) Type the URL of the endpoint http://bowtie.algorun.org/v1/run (2) Select Body from Parameters. (3) Type input=ATGCATCATGCGCCAT. (4) Click Post to initiate the request


.. image:: /images/bowtie-api-config.png
        :align: center
.. centered:: Configure Bowtie Parameters: (1) Type the URL of the endpoint http://bowtie.algorun.org/v1/config (2) Select Body from Parameters. (3) Type Report-all=on. (4) Click Post to initiate the request.
 

Packaging REACT Algorithm with AlgoRun
--------------------------------------

REACT [#f2]_ (Vera-Licona, 2014), is a software tool to reverse engineer gene regulatory networks from time series data. The source code is written in C++ and is available on GitHub at: https://github.com/veralicona/REACT/tree/master/src 
In addition, the source code includes ruby files as a helper to run the algorithm.

**STEP 1**: Add all REACT source files inside the src folder.

**STEP 2**: Add the instructions to install the C++ and ruby dependencies as well as the instructions to build REACT source code to the Dockerfile. Use the helper *ruby file ruby /home/algorithm/src/run.rb make*

.. image:: /images/react-docker.png
        :align: center
.. centered:: Source code: Dockerfile of REACT algorithm

**STEP 3**: manifest.json describes REACT algorithm. Below is how the manifest of REACT looks like.

.. image:: /images/react-manifest.png
        :align: center
.. centered:: Source Code: manifest.json of REACT algorithm (comments-skimmed)

**STEP 4**:

-	input_example.txt file includes a sample input data for users to quickly try the algorithm. Copy and paste an example from: http://react.algorun.org/algorun_info/input_example.txt 
-	output_example.txt file includes a sample of the expected output for the same input. The above input produces an output of the format: http://react.algorun.org/algorun_info/output_example.txt 


**STEP 5**:

-	From the directory where the Dockerfile exists, build REACT container using:

.. code-block:

    docker build -t react .
    
**[EXTRA STEP]** Expose REACT Parameters:

REACT algorithm uses default values for different parameters. To expose these parameters to the user, include them in the manifest file in the “algo_parameter” key as in the below picture.

.. image:: /images/react-parameters.png
        :align: center
.. centered:: REACT parameters in the manifest file

Parameters can be changed by clicking on “Change Parameters” button from the web interface. Visit http://react.algorun.org for the final version of REACT running inside AlgoRun standard container.

Find the complete example on AlgoRun GitHub repository (https://github.com/algorun/algorun/tree/master/examples/REACT).

 
Packaging KS Algorithm with AlgoRun
--------------------------------------

KS [#f3]_ Kavvadias-Stavropoulos algorithm (Kavvadias, 2005) generates all minimal hitting sets (traversals) of a hypergraph. The source code is written in Pascal and is available on Murakami and Uno's repository.

As the source code is using a dialect of Pascal that is not compatible with the modern compiler, download a helper executable that has been written to come over that problem: https://github.com/algorun/algorun/tree/master/examples/ks
For your convenience, the repository above includes KS algorithm source code as well.

**STEP 1**: Add KS thg.pas source file with the helper mhs file inside the src folder. The helper 

**STEP 2**: Add the instructions to install the C++ dependencies and Pascal compiler to the Dockerfile. As the helper is written in python, add the instructions to install the python dependencies as well. After that, navigate to the src directory and compile the source code file pc thg.pas 

Adding the instructions to install *python-pip* dependency helps in installing other python packages as simplejson in a much easier way.

.. image:: /images/ks-docker.png
        :align: center
.. centered:: Source Code: Dockerfile of KS algorithm


**STEP 3**: manifest.json describes KS algorithm. Below is how the manifest of KS looks like.

.. image:: /images/ks-manifest.png
        :align: center
.. centered:: Source Code: manifest.json of KS algorithm (comments-skimmed)


**STEP 4**:

-	input_example.txt: Copy and paste the following sample input:

.. code-block:: json

    {
        "sets": [
	       [1, 2, 5],
	       [3, 2, 4],
	       [1, 3]
            ]
    }

-	output_example.txt: Copy and paste the following sample output:

.. code-block:: json
    
    {"transversals": [[2], [3, 4], [4, 5]], "timeTaken": 0.002045721, "sets": [[1, 2, 5], [3, 2, 4], [1, 3]]}

**STEP 5**:

-	From the directory where the Dockerfile exists, build KS container using:

.. code-block:: shell

    docker build -t ks .


.. rubric:: References

.. [#f1] Langmead, B. et al. (2009). Ultrafast and memory-efficient alignment of short DNA sequences to the human genome. Genome Biology, 10:R25.

.. [#f2] Vera-Licona, P., Jarrah, A.S., Garcia, LD., Mcgee, J., Laubenbacher, R. (2014): An Algebra-Based Method for Inferring Gene Regulatory Networks. BMC Systems Biology, 8:37.

.. [#f3] Kavvadias D. and Stavropoulos E. (2005): An Efficient Algorithm for the Transversal Hypergraph Generation. J. of Graph Alg & App, 9:2, 239-264.
