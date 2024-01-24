 <template>
  <div v-if="player" :id="'selected-player'">
    <div :id="'player-name'">{{ this.player.name }}</div>
    <div :class="'player-id'">{{ player.id }}</div>
    <div :id="'player-status'">
      <label :for="'checkbox'" :id="'checkbox-label'">
        <input
          type="checkbox"
          :id="'checkbox'"
          :checked="this.player.isActive"
          @change="handleCheckboxChange"
        />
        <span class="checkmark"></span>
        {{ this.player.isActive ? 'active' : 'inactive' }}
      </label>
    </div>
    <button
      class="btn-update"
      :disabled="!this.valueChanged"
      @click="updatePlayer"
    >
      Update
    </button>
    <button class="btn-delete" @click="deletePlayer">Delete</button>
  </div>
</template>

<script>
export default {
  props: {
    player: Object,
  },
  data() {
    return {
      valueChanged: false,
    };
  },
  methods: {
    handleCheckboxChange() {
      this.valueChanged = !this.valueChanged;
      this.player.isActive = !this.player.isActive;
    },
    updatePlayer() {
      this.valueChanged = !this.valueChanged;
      this.$emit('put-player', !this.player.isActive);
    },
    deletePlayer() {
      this.$emit('delete-player', this.player.id);
    },
  },
};
</script>
