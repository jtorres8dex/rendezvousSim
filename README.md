# rendezvousSim

The Rendezvous Algorithm is a canonical problem in Graph Theoretic Control that recursively drives each agent to the centroid of its neighbors. IFF the network can be considered a connected graph, convergence to a single point is guarunteed. 

Credit to Dr. Yancy Diaz Demercado for teaching me all the theory used in this at University of Maryland. 

A Paper also used to build this: An Object-Oriented Simulation Architecture, American Institute of Aeronautics and Astronautics. 


## Motive for this project

I created project this shortly after starting my first full time job as an autonomy software engineer and realizing those Intro to Programming courses taught in the Mechanical Engineering department are rather sparse in their exposure to what C++ has to offer. This is a euphemism for saying I had absolutely no idea how to code the right way. With exposure being the best teacher, I wanted to build something in my own time to better understand core concepts such as the C++ memory model and all the useful tools provided by the language.

## Design

The core idea of this project is maintaining independence between different components of an autonomous systems, establishing a standardized interface between these components to enable smoother advancements in the development process. 

For this project, I focused on 3 main components:

- Vehicles: the physical platform being controlled by the platform. Currently is a stupidly simple unicycle dynamics model. Easily extensible to use something like Simulink to plug in an external model.

- Agents: the autonomous entity controlling the physical vehicle. Observes it's state and decides an action.

- Simulation: the runtime executive that wraps these interactions into discrete instances, handling user defined configurations and interactions between various simulation entities.

> NOTE: there are np.inf improvements that could be made to this. Likely some shortsightedness in the design as well. I wrote this before ever getting any official software engineering training, in school or professionally. That was the point. It was invaluable in learning how to code. 

