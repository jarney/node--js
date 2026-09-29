NodeJS Documentation
====================

NodeJS is a node-based meta programming language
and development environment.  Instead of being based
on lines of code in a text-based format, NodeJS
prefers to model code as a directed acyclic graph
consisting of nodes connected by edges.  Data flows
along the edges and is processed inside nodes.

This provides a very natural and visual way to represent
programming in a way that reflects the flow of data in a machine.

This way of modelling code does away with the traditional
approaches to parsing using a combination of tokenization and
some flavor of LR parsing.  Instead, the program is directly
represented as a data flow graph which fits well with
the representations used by typical compiler IR (intermediate representation).
