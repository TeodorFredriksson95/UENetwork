# Mindmap Notes
This file serves as a "write down as you go"-kind of study note. A more well structured architecture/disposition will be provided as the notes get more substantial. The notes are written primarily in an Unreal Engine 5+ context. Networking concepts between Unreal and Unity are similar, but implementation vary. For more information regarding Unity's implementation of the below notes, I recommend reading up on Unity's ***Lobby***, ***Relay*** and ***Netcode*** frameworks.

## Quick Notes

### RPC & Replication
- Low-level customizable high-level abstraction that handles the process of synchronizing data and procedure calls between server and clients.
- In competetive multiplayer games the server is usually considered server **authoritative**. This means that the clients send requests in the form of *remote procedure calls* (RPC) in order to perform actions, while the server decides whether or not these requests are considerd valid.
- Generally, the server does not handle rendereing of visuals. Rather, the server communicates state through data. This data can then be used on the client side to update the rendered state. This kind of simulation is a close approximation to the state that is actually held on the server. As communication needs to happen between server and client, a true replication of the actual server state is virtually impossible, as the process of communication takes time in some shape or manner, and can be affected things such as latency and package loss. This is not an issue on a single-player game, as the state is owned locally as opposed to relying on a network.


### Server - Client

#### Server
- Server = centralized machine enabling distinct connections between different machines.
- Shares game state with clients and provides necessary tools required for clients to communicate with each other.
- Processes gameplay

**Listen Server**
- Allows a client to act as both client and server. A client who is also the server is considered a *host*.
- Quick and easy to set up.
- Can offer an unfair advantage to the host as the host plays on the server directly, reducing communcation times between server and client.
- Requires more computation for the host as it runs the server and client-replication processes.
- Suitable for smaller, non-highly-competitive games.
- **Suitable game types:** Smaller games which usually implement some form of visual interface in order to establish client-server responsibility. An example of a setup would be a game where the player can either search for available games (servers) to join, or create a game (server) of their own, which others can join. Client-host relation is established during this setup.

**Dedicated Server**
- Only accepts remote connections. No client is allowed to exist locally on the server.
- More expensive.
- Requires more advanced configuration.
- Can be optimized further than *'Listen Server'* as it does not handle client-only rendering.
- Better suited for games featuring large amounts of players, high level competition or games that otherwise require further reliability.
- **Suitable game types:** Larger online games such a MMO's, MOBA's or fast-paced shooters.
  

#### Client
- Clients = machines connected to the server
- Renders gameplay decided by server.
