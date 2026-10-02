#define NONARY_NODE(prefix, name, category, description, outname, outtype) \
Builtins::RegistryItemPtr                                                     \
Builtins::f_##prefix##_##name() {                                             \
    auto nodeType = std::make_unique<NodeType>(#name, description, category);   \
    nodeType->addOutputPort(std::make_unique<NodePort>(outtype), outname);     \
    nodeType->setProcessor(f_##prefix##_##name##_process);                         \
    return nodeType;                                                               \
}                                                                               \
void									        \
Builtins::f_##prefix##_##name##_process(                                      \
    const Node & node,                                                 \
    const NodePortData & input,                                            \
    NodePortData & output                                                  \
    )						                                

#define UNARY_NODE(prefix, name, category, description, inname, atype, outname, outtype) \
Builtins::RegistryItemPtr                                                     \
Builtins::f_##prefix##_##name() {                                             \
    auto nodeType = std::make_unique<NodeType>(#name, description, category);   \
    nodeType->addInputPort(std::make_unique<NodePort>(atype), inname);         \
    nodeType->addOutputPort(std::make_unique<NodePort>(outtype), outname);     \
    nodeType->setProcessor(f_##prefix##_##name##_process);                         \
    return nodeType;                                                               \
}                                                                               \
void									        \
Builtins::f_##prefix##_##name##_process(                                      \
    const Node & node,                                                 \
    const NodePortData & input,                                            \
    NodePortData & output                                                  \
    )

#define BINARY_NODE(prefix, name, category, description, a, atype, b, btype, outname, outtype) \
Builtins::RegistryItemPtr                                                     \
Builtins::f_##prefix##_##name() {                                             \
    auto nodeType = std::make_unique<NodeType>(#name, description, category);   \
    nodeType->addInputPort(std::make_unique<NodePort>(atype), a);             \
    nodeType->addInputPort(std::make_unique<NodePort>(btype), b);             \
    nodeType->addOutputPort(std::make_unique<NodePort>(outtype), outname);     \
    nodeType->setProcessor(f_##prefix##_##name##_process);                         \
    return nodeType;                                                               \
}                                                                               \
void									        \
Builtins::f_##prefix##_##name##_process(                                      \
    const Node & node,                                                 \
    const NodePortData & input,                                            \
    NodePortData & output                                                  \
    )

