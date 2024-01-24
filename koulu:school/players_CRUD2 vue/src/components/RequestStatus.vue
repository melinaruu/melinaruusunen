 <script>
 export default {
   props: {
     status: String,
   },
 };
 </script>
 
 <template>
   <div :id="'request-status'">
       <slot name="status"></slot>
   </div>
 </template>