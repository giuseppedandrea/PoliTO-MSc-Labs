use dot_dsl::graph::Graph;
use dot_dsl::graph::graph_items::edge::Edge;
use dot_dsl::graph::graph_items::node::Node;

fn main() {
    let attrs = vec![
        ("label", "MyGraph"),
        ("bgcolor", "grey"),
    ];

    let nodes = vec![
        Node::new("a").with_attrs(&[("color", "red")]),
        Node::new("b").with_attrs(&[("color", "blue")]),
        Node::new("c"),
    ];

    let edges = vec![
        Edge::new("a", "b").with_attrs(&[("color", "green")]),
        Edge::new("a", "c"),
    ];

    let graph = Graph::new()
        .with_attrs(&attrs)
        .with_nodes(&nodes)
        .with_edges(&edges);

    println!("{}", graph);
}