<template>
  <div>
    <a v-if="isLoggedIn" @click="logout">Logout</a>
    <a v-else @click="toggleView">{{ isLoginView ? 'Go to register' : 'Go to login' }}</a>

    <form :id="'auth-form'" v-if="isLoginView || isRegisterView" @submit.prevent="submitForm">
      <label for="username">Username:</label>
      <input
        id="username"
        name="auth-username"
        type="text"
        v-model="username"
        required
      />

      <label for="password">Password:</label>
      <input
        id="password"
        name="auth-password"
        type="password"
        v-model="password"
        required
      />

      <button class="btn-auth" type="submit">
        {{ isLoginView ? 'Login' : 'Register' }}
      </button>
    </form>
  </div>

</template>

<script>
export default {
  props: {
    isLoggedIn: Boolean,
  },
  data() {
    return {
      isLoginView: true,
      isRegisterView: false,
      username: '',
      password: '',
    };
  },
  methods: {
    logout() {
      this.$emit('logout');
    },
    toggleView() {
      this.isLoginView = !this.isLoginView;
      this.isRegisterView = !this.isRegisterView;
    },
    submitForm() {
      if (this.isLoginView) {
        this.$emit('login', {username: this.username, password: this.password});
      } else {
        this.$emit('register', {username: this.username, password: this.password});
      }
      this.username='';
      this.password='';
    },
  },
};

</script>