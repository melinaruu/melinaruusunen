 <script>
export default {
  data() {
    return {
      newPlayerName: '',
    };
  },
  methods: {
    submitPlayer() {
      if (this.newPlayerName.trim() !== '') {
        this.$emit('add-player', this.newPlayerName);
        this.newPlayerName = '';
      }
    },
  },
};
</script>

<template>
  <form :id="'submit-player'" @submit.prevent="submitPlayer">
    <label :for="'input-player'">New Player:</label>
    <input 
      :id="'input-player'"
      v-model="newPlayerName"
      placeholder="Enter player name"
      required
    />
    <button type="submit" class="btn-add">Add Player</button>
  </form> 
</template>