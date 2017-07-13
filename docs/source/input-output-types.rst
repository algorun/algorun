Input/Output Types
===================

algorun:DNASequence 
####################
**Usage**: A Short DNA Sequence

**Example**:

.. code-block:: json

    ATGCATCATGCGCCAT
    
algorun:AlignedDNASequence  
############################
**Usage**: A short DNA sequence algined to large genome

**Example**:

.. code-block:: json

    0	-	gi|110640213|ref|NC_008253.1|	148810	ATGGCGCATGATGCAT	IIIIIIIIIIIIIIII	0	10:A>G,13:C>G
    

superadam:PolynomialDynamicalSystemSet  
########################################
**Usage**: A Set of polynomial dynamical systems

**Example**:

.. code-block:: json

    {
    "description": "Polynomial model"",
    "fieldCardinality": 2,
    "name": "priorModel",
    "type": "PolynomialDynamicalSystemSet",
    "updateRules": [
	{
	  "functions": [
	      {
		"inputVariables": ["x1","x2"],
		"polynomialFunction": "x1*x2"
	      }
	    ],
	  "target": "x1"
	},
	{
	  "functions": [
	      {
		"inputVariables": ["x1"],
		"polynomialFunction": "x1+1"
	      },
	      {
		"inputVariables": ["x1","x2"],
		"polynomialFunction": "x1*x2"
	      }
	    ],
	  "target": "x2"
	},
	{
	  "functions": [
	      {
		"inputVariables": ["x1","x2","x3"],
		"polynomialFunction": "x3^2+x1"
	      }
	    ],
	  "target": "x3"
	},
	{
	  "functions": [
	      {
		"inputVariables": ["x1","x3"],
		"polynomialFunction": "x3^2+x1+x2"
	      }
	    ],
	  "target": "x4"
	}
      ]
      }
    
superadam:DiscreteDynamicalSystemSet  
########################################
**Usage**: a set of discrete dynamical systems

**Example**:

.. code-block:: json

    {
	"type": "DiscreteDynamicalSystemSet",
	"description": "a description",
	"simulationName": "a name",
	"updateRules": [
	    {
		"target": "CAP",
		"functions": [
		    {
			"inputVariables": ["CAP"],
			"transitionTable": [
				[[0],0],
				[[1],1]
			    ]                         
		    }
		]
	    },
	    {
		"target": "mRNA",
		"functions": [
		    {
			"inputVariables": ["CAP","LacI","mRNA"],
			"transitionTable": [
				[[0,0,0],0],
				[[0,0,1],1],
				[[0,1,0],0],
				[[0,1,1],0],
				[[0,2,0],0],
				[[0,2,1],0],
				[[1,0,0],1],
				[[1,0,1],0],
				[[1,1,0],0],
				[[1,1,1],0],
				[[1,2,0],0],
				[[1,2,1],0]      
			  ]
		    }
		 ]   
	    },
	    {
		"target": "LacY",
		"functions": [
		    {
			"inputVariables": ["mRNA","LacY"],
			"transitionTable": [
				[[0,0],0],
				[[0,1],1],
				[[1,0],1],
				[[1,1],1]
			   ]
		     }
		]
	    },
	    {
		"target": "LacZ",
		"functions": [
		    {
			"inputVariables": ["mRNA","LacZ"],
			"transitionTable": [
				[[0,0],0],
				[[0,1],1],
				[[1,0],1],
				[[1,1],1]
			    ]
		    }
		]
	    },
	    {
		"target": "LacI",
		"functions": [
		    {
			"inputVariables": ["LacI"],
			"transitionTable": [
				[[0],0],
				[[1],1],
				[[2],2]
			 ]
		    }
		]
	    }
	],
	"variables": [
	    {
		"id": "CAP",
		"states": [0,1],
		"speed": 1
	    },
	    {
		"id": "mRNA",
		"states": [0,1],
		"speed": 1
	    },
	    {
		"id": "LacY",
		"states": [0,1],
		"speed": 1
	    },
	    {
		"id": "LacZ",
		"states": [0,1],
		"speed": 1
	    },
	    {
		"id": "LacI",
		"states": [0,1,2],
		"speed": 1
	    }  
	]
    }

    
superadam:BooleanDynamicalSystemSet 
######################################
**Usage**: a set of boolean dynamical systems

**Example**:

.. code-block:: json

    {
	"type": "BooleanDynamicalSystemSet",
	"description": "Sample Boolean Network",
	"parameters": [
	    {
	    "id": "k1",
	    "states": [0,1]
	    },
	    {
	    "id": "k2",
	    "states": [0,1]
	    }
	  ],
	"updateRules": [
	    {
		"target": "x1",
		"functions": [
		    {
			"inputVariables": ["k1","x3"],
			"booleanFunction": "k1 & x3"
		    }
		]
	    },
	    {
		"target": "x2",
		"functions": [
		    {
			"inputVariables": ["x1" , "k2"],
			"booleanFunction": "x1 | k2"
		    }
		]
	    },
	    {
		"target": "x3",
		"functions": [
		    {
			"inputVariables": ["x4","x2"],
			"booleanFunction": "x2 & !x4"
		    }
		]
	    },
	    {
		"target": "x4",
		"functions": [
		    {
			"inputVariables": ["x2","k2"],
			"booleanFunction": "x2 & k2"
		    }
		]
	    }
	],

	"variables": [
	    {
		"id": "x1",
		"states": [0,1]
	    },
	    {
		"id": "x2",
		"states": [0,1]
	    },
	    {
		"id": "x3",
		"states": [0,1]
	    },
	    {
		"id": "x4",
		"states": [0,1]
	    }
	]
    }


superadam:TimeSeriesSet 
#########################
**Usage**: A set of time series

**Example**:

.. code-block:: json

    {
    "type": "timeSeriesSet",
    "timeSeriesData": [
	{
	  "index": [],
	  "matrix": [
	      [1,0,0,0],
	      [0,1,0,1],
	      [1,1,0,0]
	    ],
	  "name": "wildtype experiment 1"
	},
	{
	  "index": [],
	  "matrix": [
	      [1,1,0,0],
	      [0,0,0,1],
	      [1,0,0,0]
	    ],
	  "name": "wildtype experiment 2"
	},
	{
	  "index": [1],
	  "matrix": [
	      [0,0,0,0],
	      [0,0,0,1],
	      [0,1,0,0],
	      [0,0,0,1]
	    ],
	  "name": "knockout experiment 1"
	},
	{
	  "index": [3],
	  "matrix": [
	      [0,1,0,0],
	      [0,1,0,1],
	      [0,1,0,1],
	      [0,0,0,0]
	    ],
	  "name": "knockout experiment 2"
	},
	{
	  "index": [2],
	  "matrix": [
	      [1,0,0,0],
	      [0,0,0,1],
	      [1,0,0,1],
	      [0,0,0,0]
	    ],
	  "name": "knockout experiment 3"
	}
      ]
    }


superadam:DirectedGraph 
#########################
**Usage**: a directed graph representation

**Example**:

.. code-block:: json

    {
    "description": "",
    "fieldCardinality": 2,
    "name": "priorReverseEngineeringNetwork",
    "type": "directedGraph",
    "edges": [
	{
	  "sources": [
	      { "score": 0.5, "source": "x1" },
	      { "score": 1, "source": "x2" },
	      { "score": 1, "source": "x3" },
	      { "score": 1, "source": "x4" }
	    ],
	  "target": "x1"
	},
	{
	  "sources": [
	      { "score": 1, "source": "x2" },
	      { "score": 1, "source": "x4" }
	    ],
	  "target": "x2"
	},
	{
	  "sources": [
	      { "score": 0.5, "source": "x1" },
	      { "score": 0.5, "source": "x2" }
	    ],
	  "target": "x3"
	},
	{
	  "sources": [
	      { "score": 0.33, "source": "x2" },
	      { "score": 0.66, "source": "x3" }
	    ],
	  "target": "x4"
	}
    ]
    }



superadam:AnnotatedGraph 
#########################
**Usage**: an annotated graph representation

**Example**:

.. code-block:: json

    {
	"type": "AnnotatedGraph",
	"description": "Sample Annotated Graph",
	"node": [                        
	   {
	      "id" : "node0",
	      "label": " 0 0 0 0 0"
	   },
	   {
	      "id" : "node1",
	      "label": " 0 0 0 0 1"
	   },
	   {
	      "id" : "node2",
	      "label": " 0 0 0 0 2"
	   },
	   {
	      "id" : "node3",
	      "label": " 0 0 0 1 0"
	   },
	   {
	      "id" : "node4",
	      "label": " 0 0 0 1 1"
	   },
	   {
	      "id" : "node5",
	      "label": " 0 0 0 1 2"
	   },
	   {
	      "id" : "node6",
	      "label": " 0 0 1 0 0"
	   },
	   {
	      "id" : "node7",
	      "label": " 0 0 1 0 1"
	   },
	   {
	      "id" : "node8",
	      "label": " 0 0 1 0 2"
	   },
	   {
	      "id" : "node9",
	      "label": " 0 0 1 1 0"
	   },
	   {
	      "id" : "node10",
	      "label": " 0 0 1 1 1"
	   },
	   {
	      "id" : "node11",
	      "label": " 0 0 1 1 2"
	   },
	   {
	      "id" : "node12",
	      "label": " 0 1 0 0 0"
	   },
	   {
	      "id" : "node13",
	      "label": " 0 1 0 0 1"
	   },
	   {
	      "id" : "node14",
	      "label": " 0 1 0 0 2"
	   },
	   {
	      "id" : "node15",
	      "label": " 0 1 0 1 0"
	   },
	   {
	      "id" : "node16",
	      "label": " 0 1 0 1 1"
	   },
	   {
	      "id" : "node17",
	      "label": " 0 1 0 1 2"
	   },
	   {
	      "id" : "node18",
	      "label": " 0 1 1 0 0"
	   },
	   {
	      "id" : "node19",
	      "label": " 0 1 1 0 1"
	   },
	   {
	      "id" : "node20",
	      "label": " 0 1 1 0 2"
	   },
	   {
	      "id" : "node21",
	      "label": " 0 1 1 1 0"
	   },
	   {
	      "id" : "node22",
	      "label": " 0 1 1 1 1"
	   },
	   {
	      "id" : "node23",
	      "label": " 0 1 1 1 2"
	   },
	   {
	      "id" : "node24",
	      "label": " 1 0 0 0 0"
	   },
	   {
	      "id" : "node25",
	      "label": " 1 0 0 0 1"
	   },
	   {
	      "id" : "node26",
	      "label": " 1 0 0 0 2"
	   },
	   {
	      "id" : "node27",
	      "label": " 1 0 0 1 0"
	   },
	   {
	      "id" : "node28",
	      "label": " 1 0 0 1 1"
	   },
	   {
	      "id" : "node29",
	      "label": " 1 0 0 1 2"
	   },
	   {
	      "id" : "node30",
	      "label": " 1 0 1 0 0"
	   },
	   {
	      "id" : "node31",
	      "label": " 1 0 1 0 1"
	   },
	   {
	      "id" : "node32",
	      "label": " 1 0 1 0 2"
	   },
	   {
	      "id" : "node33",
	      "label": " 1 0 1 1 0"
	   },
	   {
	      "id" : "node34",
	      "label": " 1 0 1 1 1"
	   },
	   {
	      "id" : "node35",
	      "label": " 1 0 1 1 2"
	   },
	   {
	      "id" : "node36",
	      "label": " 1 1 0 0 0"
	   },
	   {
	      "id" : "node37",
	      "label": " 1 1 0 0 1"
	   },
	   {
	      "id" : "node38",
	      "label": " 1 1 0 0 2"
	   },
	   {
	      "id" : "node39",
	      "label": " 1 1 0 1 0"
	   },
	   {
	      "id" : "node40",
	      "label": " 1 1 0 1 1"
	   },
	   {
	      "id" : "node41",
	      "label": " 1 1 0 1 2"
	   },
	   {
	      "id" : "node42",
	      "label": " 1 1 1 0 0"
	   },
	   {
	      "id" : "node43",
	      "label": " 1 1 1 0 1"
	   },
	   {
	      "id" : "node44",
	      "label": " 1 1 1 0 2"
	   },
	   {
	      "id" : "node45",
	      "label": " 1 1 1 1 0"
	   },
	   {
	      "id" : "node46",
	      "label": " 1 1 1 1 1"
	   },
	   {
	      "id" : "node47",
	      "label": " 1 1 1 1 2"
	   }
       ],
       "connection": [
	    ["node0", "node0"],
	    ["node1", "node1"], 
	    ["node2","node2"],
	    ["node3","node3"],
	    ["node4","node4"],
	    ["node5","node5"],
	    ["node6","node6"],
	    ["node7","node7"],
	    ["node8","node8"],
	    ["node9","node9"],
	    ["node10","node10"],
	    ["node11","node11"],
	    ["node12","node21"],
	    ["node13","node10"],
	    ["node14","node11"],
	    ["node15","node21"],
	    ["node16","node10"],
	    ["node17","node11"],
	    ["node18","node21"],
	    ["node19","node10"],
	    ["node20","node11"],
	    ["node21","node21"],
	    ["node22","node10"],
	    ["node23","node11"],
	    ["node24","node36"],
	    ["node25","node25"],
	    ["node26","node26"],
	    ["node27","node39"],
	    ["node28","node28"],
	    ["node29","node29"],
	    ["node30","node42"],
	    ["node31","node31"],
	    ["node32","node32"],
	    ["node33","node45"],
	    ["node34","node34"],
	    ["node35","node35"],
	    ["node36","node33"],
	    ["node37","node34"],
	    ["node38","node35"],
	    ["node39","node33"],
	    ["node40","node34"],
	    ["node41","node35"],
	    ["node42","node33"],
	    ["node43","node34"],
	    ["node44","node35"],
	    ["node45","node33"],
	    ["node46","node34"],
	    ["node47","node35"]
       ]
       }

superadam:SteadyStates 
#######################
**Usage**: steady states of boolean dynamical system set

**Example**:

.. code-block:: json

    {
    "type": "SteadyStates",
    "description": "steady states of boolean dynamical system set",
    "steadystates": {
                "idorder": ["x1","x2","x3","x4"],
                "value": [
                            [0,0,0,0],
                            [1,1,1,0]
                        ]
                    }
        }
