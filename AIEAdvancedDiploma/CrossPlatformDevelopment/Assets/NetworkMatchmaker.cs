////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - NetworkMatchmaker.cs
////////////////////////////////////////////////////////////////////////////////////////

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

/// <summary>
/// Manages and connects to Photon Unity Networking
/// </summary>
public class NetworkMatchmaker : Photon.PunBehaviour
{
	int m_roomJoined = 0;				   // Index of room currently joined
	const int m_maxPlayersPerRoom = 10;    // Number of players allowed per room
	const int m_maxPlayersAllowed = 20;    // Maximum allowed connections with current Photon plan
	List<string> m_roomNames;              // List of all available rooms to join

	/// <summary>
    /// Initialises the matchmaker
    /// </summary>
	void Start()
	{
		int maxRooms = Mathf.CeilToInt(
			(float)m_maxPlayersAllowed /(float) m_maxPlayersPerRoom);

		m_roomNames = (from i in Enumerable.Range(0, maxRooms) 
		               select i.ToString()).ToList();

		Debug.Log(m_roomNames);
	}

    /// <summary>
    /// Gets the status of the connection
    /// </summary>
    public string GetNetworkStatus()
    {
        return PhotonNetwork.connectionStateDetailed.ToString();
    }

    /// <summary>
    /// Gets whether the network is connected to a room
    /// </summary>
    public bool IsConnectedToRoom()
    {
        return PhotonNetwork.inRoom;
    }

    /// <summary>
    /// Gets whether the network is connected to a room
    /// </summary>
    public bool IsConnectedToLobby()
    {
        return PhotonNetwork.insideLobby;
    }

	/// <summary>
    /// Gets whether the player is allowed to connect to the game
    /// </summary>
	public bool CanConnectToNetwork()
	{
		return PhotonNetwork.countOfPlayers < m_maxPlayersAllowed;
	}

	/// <summary>
    /// Connects the player to an open room
    /// </summary>
	public void JoinGameRoom()
	{
		RoomOptions roomOptions = new RoomOptions();
		roomOptions.isVisible = true;
		roomOptions.isOpen = true;
		roomOptions.maxPlayers = m_maxPlayersPerRoom;
		
		PhotonNetwork.JoinOrCreateRoom(m_roomNames[m_roomJoined], 
		                               roomOptions, TypedLobby.Default);
	}

    /// <summary>
    /// On player joining the lobby
    /// </summary>
    public override void OnJoinedLobby()
    {
		Debug.Log("Player Joined lobby");
    }
	
	/// <summary>
	/// On player joining a new room
	/// </summary>
	public override void OnJoinedRoom()
    {
		Debug.Log("Player Joined room");

		//GameObject player = PhotonNetwork.Instantiate("", Vector3.zero, Quaternion.identity, 0);
    }
	
	/// <summary>
    /// On player leaving a room
	/// </summary>
	public override void OnLeftRoom()
	{
		Debug.Log("Player Left room");
		m_roomJoined = 0;
	}

	/// <summary>
    /// On player failed to join room
	/// </summary>
	public override void OnPhotonJoinRoomFailed(object[] codeAndMsg)
    {
        Debug.Log("Could not join room " + m_roomJoined.ToString());
		++m_roomJoined;
		JoinGameRoom();
    }

	/// <summary>
	/// Called when something causes the connection to fail (after it was established)
	/// </summary>
	public override void OnConnectionFail(DisconnectCause cause)
	{
		Debug.Log("Player Disconnected");
	}

    /// <summary>
    /// Attemps to re-connect if needed
    /// </summary>
    void Update()
    {
        if(!IsConnectedToLobby())
        {
            PhotonNetwork.ConnectUsingSettings(Utilities.GameVersion());
        }
    }
}