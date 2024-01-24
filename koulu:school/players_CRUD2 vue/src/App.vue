 <template>
  <div>
    <RequestStatus>
      <template v-slot:status>
        {{ requestStatus }}
      </template>
    </RequestStatus>

    <AuthUser
      v-if="!isLoggedIn"
      :isLoggedn="isLoggedIn"
      @login="loginUser"
      @register="registerUser"
      @logout="logoutUser"
    ></AuthUser>

    <ListPlayers v-if="isLoggedIn" :getPlayer="fetchPlayer" :players="players"></ListPlayers>

    <SelectedPlayer 
      v-if="isLoggedIn" 
      :player="selectedPlayer" 
      @delete-player="deletePlayer"
      @put-player="updatePlayer"
    ></SelectedPlayer>

    <AddPlayer v-if="isLoggedIn" @add-player="addPlayer"></AddPlayer>
  </div>
</template>

<script>
import ListPlayers from './components/ListPlayers.vue';
import RequestStatus from './components/RequestStatus.vue';
import SelectedPlayer from './components/SelectedPlayer.vue';
import AddPlayer from './components/AddPlayer.vue';
import AuthUser from './components/AuthUser.vue';

const REQ_STATUS = {
  loading: "Loading...",
  success: "Finished!",
  error: "An error has occurred!!!",
};

export default {
  components: {
    RequestStatus,
    ListPlayers,
    SelectedPlayer,
    AddPlayer,
    AuthUser,
    },
  data() {
    return {
      players: [],
      selectedPlayer: null,
      requestStatus: REQ_STATUS.loading,
      isLoggedIn: false,
      username: '',
      password: '',
    };
  },
  created() {
    if (this.isLoggedIn) {
      this.fetchAllPlayers()
    }
  },
  methods: {
    async fetchAllPlayers() {
      try {
        this.requestStatus = REQ_STATUS.loading;
        const response = await fetch('http://localhost:3001/api/players', {
          headers: {
          'Authorization': `Basic ${window.btoa(`${this.username}:${this.password}`)}`,
          },
        });
        const data = await response.json();
        this.players = data;
        this.requestStatus = REQ_STATUS.success;
      } catch (error) {
        console.error(error);
        this.requestStatus = REQ_STATUS.error;
      }
    },
    async fetchPlayer(playerId) {
      try {
        this.requestStatus = REQ_STATUS.loading;
        const response = await fetch(`http://localhost:3001/api/players/${playerId}`, {
          headers: {
          'Authorization': `Basic ${window.btoa(`${this.username}:${this.password}`)}`,
          },
        });
        const data = await response.json();
        this.selectedPlayer = data;
        this.requestStatus = REQ_STATUS.success;
      } catch (error) {
        console.error(error);
        this.requestStatus = REQ_STATUS.error;
      }
    },
    async addPlayer(newPlayerName) {
      try {
        this.requestStatus = REQ_STATUS.loading;
        const response = await fetch('http://localhost:3001/api/players', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
            'Authorization': `Basic ${window.btoa(`${this.username}:${this.password}`)}`,
          },
          body: JSON.stringify({name: newPlayerName}),
        });
        const data = await response.json();
        this.players.push(data);
        this.requestStatus = REQ_STATUS.success;
      } catch(error) {
        console.error(error);
        this.requestStatus = REQ_STATUS.error;
      }
    },
    async deletePlayer(playerId) {
      try {
        this.requestStatus = REQ_STATUS.loading;
        const response = await fetch(`http://localhost:3001/api/players/${playerId}`, {
          method: 'DELETE',
          headers: {
          'Authorization': `Basic ${window.btoa(`${this.username}:${this.password}`)}`,
          },
        });
        this.players = this.players.filter(player => player.id !== playerId);
        this.requestStatus = REQ_STATUS.success;
      } catch (error) {
        console.error(error);
        this.requestStatus = REQ_STATUS.error;
      }
    },
    async updatePlayer(updatedStatus) {
      try {
        if (this.selectedPlayer) {
          const playerId = this.selectedPlayer.id;
          this.requestStatus = REQ_STATUS.loading;
          const response = await fetch(`http://localhost:3001/api/players/${playerId}`, {
            method: 'PUT',
            headers: {
              'Content-Type': 'application/json',
              'Authorization': `Basic ${window.btoa(`${this.username}:${this.password}`)}`,
            },
            body: JSON.stringify({isActive: updatedStatus}),
          });
          const data = await response.json();
        
          this.selectedPlayer.isActive = !updatedStatus;

          this.requestStatus = REQ_STATUS.success;
        }
      } catch (error) {
        console.error(error);
        this.requestStatus = REQ_STATUS.error;
      }
    },
    async registerUser(credentials) {
      try {
        this.requestStatus = REQ_STATUS.loading;
        const response = await fetch('http://localhost:3001/api/players', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({username: credentials.username}),
        });
        this.username=credentials.username;
        this.password=credentials.password;
        await this.fetchAllPlayers();
        this.isLoggedIn = true;
        this.requestStatus = REQ_STATUS.success;
      } catch(error) {
        console.error(error);
        this.requestStatus = REQ_STATUS.error;
      }
    },
    async loginUser(credentials) {
      try {
        this.requestStatus = REQ_STATUS.loading;
        const response = await fetch('http://localhost:3001/api/players', {
          method: 'GET',
          headers: {
            'Authorization': `Basic ${window.btoa(`${credentials.username}:${credentials.password}`)}`,
          },
        });
        this.username=credentials.username;
        this.password=credentials.password;
        await this.fetchAllPlayers();
        this.isLoggedIn = true;
        this.requestStatus = REQ_STATUS.success;
      } catch(error) {
        console.error(error);
        this.requestStatus = REQ_STATUS.error;
      }
    },
    logoutUser() {
      this.requestStatus = REQ_STATUS.loading;
      this.players = [];
      this.selectedPlayer = null;
      this.isLoggedIn = false;
      this.username= '';
      this.password='';
      this.requestStatus = REQ_STATUS.success;
    },
  },
};
</script>