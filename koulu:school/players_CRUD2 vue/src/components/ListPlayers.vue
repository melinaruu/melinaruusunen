
<script>
import ListPlayer from './ListPlayer.vue';

export default {
  props: {
    getPlayer: Function,
    players: Array,
  },
  components: {
    ListPlayer,
  },
};
</script>

<template>
  <ul :id="'players-list'">
    <ListPlayer
      v-for="player in players"
      :key="player.id"
      :player="player"
      @player-clicked="getPlayer"
    />
  </ul>

</template>