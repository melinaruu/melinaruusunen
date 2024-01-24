 <script>
 export default {
   props: {
     player: Object,
   },
   methods: {
     handlePlayerClick() {
       this.$emit("player-clicked", this.player.id);
     },
   },
 };
 </script>
 
 <template>
   <li :id="'player-' + player.id">
     <a @click="handlePlayerClick" tabindex="0">{{ player.name }}</a>
   </li>
 </template>
 
 <style scoped>
   a {
     cursor: pointer;
   }
 </style>