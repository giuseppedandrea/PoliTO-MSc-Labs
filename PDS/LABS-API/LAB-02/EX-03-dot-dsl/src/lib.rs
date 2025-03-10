pub mod graph {
    use std::collections::HashMap;
    use std::fmt::{Display, Formatter};
    use crate::graph::graph_items::edge::Edge;
    use crate::graph::graph_items::node::Node;

    pub struct Graph {
        pub nodes: Vec<Node>,
        pub edges: Vec<Edge>,
        pub attrs: HashMap<String, String>,
    }

    impl Graph {
        pub fn new() -> Self {
            Graph {
                nodes: vec![],
                edges: vec![],
                attrs: HashMap::new(),
            }
        }

        pub fn with_nodes(mut self, nodes: &[Node]) -> Self {
            self.nodes = nodes.to_vec();
            self
        }

        pub fn with_edges(mut self, edges: &[Edge]) -> Self {
            self.edges = edges.to_vec();
            self
        }

        pub fn with_attrs(mut self, attrs: &[(&str, &str)]) -> Self {
            self.attrs = attrs.iter().map(|(k, v)| { (k.to_string(), v.to_string()) }).collect();
            self
        }

        pub fn get_node(&self, name: &str) -> Option<&Node> {
            self.nodes.iter().find(|n| { n.name == name })
        }
    }

    impl Display for Graph {
        fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
            write!(f, "graph {{\n")?;

            if !self.attrs.is_empty() {
                let attrs = self.attrs
                    .iter()
                    .map(|(k, v)| { format!("{} = \"{}\"", k, v) })
                    .collect::<Vec<_>>()
                    .join(", ");
                write!(f, "\tgraph [{}]\n", attrs)?;
            }

            if !self.nodes.is_empty() {
                let nodes = self.nodes
                    .iter()
                    .map(|n| {
                        if !n.attrs.is_empty() {
                            let attrs = n.attrs
                                .iter()
                                .map(|(k, v)| { format!("{} = \"{}\"", k, v) })
                                .collect::<Vec<_>>()
                                .join(", ");
                            format!("{} [{}]", n.name, attrs)
                        } else {
                            format!("{}", n.name)
                        }
                    })
                    .collect::<Vec<_>>();
                for n in nodes {
                    write!(f, "\t{}\n", n)?;
                }
            }

            if !self.edges.is_empty() {
                let edges = self.edges
                    .iter()
                    .map(|e| {
                        if !e.attrs.is_empty() {
                            let attrs = e.attrs
                                .iter()
                                .map(|(k, v)| { format!("{} = \"{}\"", k, v) })
                                .collect::<Vec<_>>()
                                .join(", ");
                            format!("{} -- {} [{}]", e.node1, e.node2, attrs)
                        } else {
                            format!("{} -- {}", e.node1, e.node2)
                        }
                    })
                    .collect::<Vec<_>>();
                for e in edges {
                    write!(f, "\t{}\n", e)?;
                }
            }

            write!(f, "}}")?;

            Ok(())
        }
    }

    pub mod graph_items {
        pub mod edge {
            use std::collections::HashMap;

            #[derive(Debug, Clone, PartialEq)]
            pub struct Edge {
                pub node1: String,
                pub node2: String,
                pub attrs: HashMap<String, String>,
            }

            impl Edge {
                pub fn new(node_in: &str, node_out: &str) -> Self {
                    Edge {
                        node1: node_in.to_string(),
                        node2: node_out.to_string(),
                        attrs: HashMap::new(),
                    }
                }

                pub fn with_attrs(mut self, attrs: &[(&str, &str)]) -> Self {
                    self.attrs = attrs.iter().map(|(k, v)| { (k.to_string(), v.to_string()) }).collect();
                    self
                }
            }
        }

        pub mod node {
            use std::collections::HashMap;

            #[derive(Debug, Clone, PartialEq)]
            pub struct Node {
                pub name: String,
                pub attrs: HashMap<String, String>,
            }

            impl Node {
                pub fn new(name: &str) -> Self {
                    Node {
                        name: name.to_string(),
                        attrs: HashMap::new(),
                    }
                }

                pub fn with_attrs(mut self, attrs: &[(&str, &str)]) -> Self {
                    self.attrs = attrs.iter().map(|(k, v)| { (k.to_string(), v.to_string()) }).collect();
                    self
                }

                pub fn get_attr(&self, name: &str) -> Option<&str> {
                    self.attrs.get(name).map(|v| { v.as_ref() })
                }
            }
        }
    }
}
