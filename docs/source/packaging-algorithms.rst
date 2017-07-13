Packaging Algorithms
====================

AlgoRun is a docker-based software container template designed to package computational algorithms.
These pages show steps of how to create an AlgoRun container of your implemented algorithm.

Packaged Algorithms with AlgoRun before?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Don't forget to use **docker pull algorun/algorun** before you start packaging algorithms, in order to get the latest update of AlgoRun Docker image.


1. Download and Install Docker
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Before starting, download and install Docker on your local machine. Docker can be installed on Mac OS, 
Linux as well as Windows. Follow the instructions on: https://docs.docker.com/v1.8/installation/

2.  Download AlgoRun
^^^^^^^^^^^^^^^^^^^^^
Download AlgoRun from: https://github.com/algorun/skeleton


3. Create an AlgoRun-based Docker Image
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Unzip the downloaded skeleton-master.zip file. The resulting folder has the following structure:

.. image:: /images/skeleton.png
        :align: center

* A  Dockerfile  is  a  text  document  that  contains  all  commands  needed  to  build  the  software container. Docker builds a software container automatically by reading the instructions from the Dockerfile.
* AlgoRun template container uses the *src* and *algorun_info* folders to deposit all your source code and to describe the  implemented algorithm in a standard format.

**STEP 1**: Add all source code files of your algorithms in to the *src* folder

**STEP 2**: Edit  the *Dockerfile* to  make  sure  your  algorithm  dependencies  will  get  installed  in  the  container.  AlgoRun  is  based  on  Ubuntu  15.10  Linux  system  so  you  can  leverage  Ubuntu  packaging system to get your dependencies installed

For more information about Dockerfile, please refer to the Docker documentation (https://docs.docker.com/v1.8/reference/builder/)

**STEP 3**: Edit  the *manifest.json* file  inside  the *algorun_info* folder. Below is an example of the manifest file.

.. image:: /images/manifest.png
        :align: center

In addition to adding algorithm’s information, the following fields are necessary for AlgoRun to correctly execute the algorithm source code:

* “algo_exec”: is the command used to start algorithm executed.
* “algo_input”: is how the algorithm reads the input data.
* “algo_output”: is the path of the file where the algorithm outputs its result or stdout if the algorithm prints the result to the standard output stream.

Command line options can be exposed in the “algo_parameters” field (Refer to examples tab for a  detailed  example using  parameters). AlgoRun website uses “input_type” and “output_type” to  easily identify  algorithms  that  can  communicate  together. Please  refer  to http://algorun.org/input-output-types to see what input and output types you should use. Users can also download and use the algorithm Docker image locally from Docker Hub if the value “algo_image” is provided. 

**STEP 4**: Provide input and output examples in the input_example and output_example folders respectively.

**STEP 5**: Build the algorithm container from the command line using docker build command:
`docker build -t <algorithm_name> .`

4. Run the algorithm
^^^^^^^^^^^^^^^^^^^^^
Once the algorithm container has been created, it must first be deployed before the user can start using the packaged algorithm. The container can be deployed with the following command:

`docker run -p 31331:8765 --name <container_name> <algorithm_name>`

Now that the algorithm container has been deployed on your local machine (localhost), AlgoRun provides the user with three different ways to run the algorithm.

4.1 Web User Interface
#######################
The easiest and quickest way to run the packaged algorithm is to open the web browser and type http://localhost:31331

.. image:: /images/web-interface.png
        :align: center
.. centered:: The web user interface of an algorithm packaged with AlgoRun. Type in the address http://localhost:31331 in a web browser to open the web page of the running algorithm container.

4.2 Web API
############
A web API is an Application Programming Interface (API) used to offer programmatic access to remote resources or services (in our case “computations from an algorithm”) that can be accessed by clients such as web browsers or any http-enabled third-party applications. AlgoRun containers are pre-included with a RESTful API [#f1]_ that allows access to the computation through the traditional HTTP POST request. Clients communicate to web APIs through a request/response protocol. To ask the web service to perform a computation, a client sends an HTTP request. The body of the request includes necessary input data for the algorithm behind the web service to start. The response of the web service includes the result of the computation.

.. image:: /images/web-api.png
        :align: center
.. centered:: AlgoRun Web API Communication Scheme.

The HTTP request should be sent to a specific address, which is called an endpoint. The two main endpoints exposed by AlgoRun containers are shown below.

+--------------------------+----------------------------+---------------------+-------------------------------+
|       Endpoint           |            Usage           |  Reuqest Parameters |            Response           |
+==========================+============================+=====================+===============================+
| **HTTP POST /v1/run**    | used to run the algorithm  | input: <input_data> | The result of the computation |
|                          | on a given input data      |                     | in the body of the response   |
+--------------------------+----------------------------+---------------------+-------------------------------+
| **HTTP POST /v1/config** | used to dynamically change | <parameter_name>:   | The result of changing the    |
|                          | the parameters values      | <parameter_value>   | parameter value               |
+--------------------------+----------------------------+---------------------+-------------------------------+

By offering standardized input and output, Web APIs are particularly useful when it comes to building complex software applications as they make it easy to integrate different algorithms that usually run on different programming environments. It enhances the modularity of the software, hence increases its robustness and makes troubleshooting problems easier. You can embed computations in large software programs in just a few lines of code, removing the hassle of installing the whole algorithm environment locally.

To test a web API, Firefox Poster Plugin is a graphical user interface tool used to easily send and troubleshoot HTTP requests. See below for examples on using the above AlgoRun endpoints.

.. image:: /images/web-api-call.png
        :align: center
.. centered:: Calling Web API using Firefox Poster Plugin: (1) Type the URL of the endpoint http://localhost:31331/v1/run (2) Select Body from Parameters. (3) Type input=<paste_your_input_data_here>. (4) Click Post to initiate the request

.. image:: /images/web-api-config.png
        :align: center
.. centered:: Configuring Parameters using Web API: (1) Type the URL of the endpoint http://localhost:31331/v1/config (2) Select Body from Parameters. (3) Type <parameter_name>=<parameter_value>. (4) Click Post to initiate the request.

4.3 Command Line
###################

The traditional command line execution is still available as well.

.. code-block:: shell

    docker exec -i <container_name> /bin/algorun < sample_input.txt


5. Publish your algorithm to the AlgoRun website
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
If you packaged your algorithm with AlgoRun and want to give your algorithm more visibility, we encourage you to submit it for listing on the AlgoRun website. The AlgoRun website serves as a repository for all computational algorithms that were packaged using AlgoRun: http://algorun.org 

To submit your algorithm for listing, fill the form located at http://algorun.org/submit-algorithm

.. rubric:: Footnotes

.. [#f1] REpresentational State Transfer (REST) APIs uses Hyper Text Transfer Protocol (HTTP) requests as the main scheme of communication.