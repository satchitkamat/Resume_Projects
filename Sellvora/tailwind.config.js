export default {
  content: [
    "./index.html",
    "./src/**/*.{ts,tsx}",
  ],
  theme: {
  extend: {
      colors: {
        // Brand Colors
        sellvora: {
          darkblue: '#1E2A38',
          blue: '#1E40AF',     // Primary brand
          blueHover: '#1D4ED8',
          green: '#22C55E',    // Growth / success
          greenHover: '#16A34A',

          // Backgrounds  
          page: '#f1f5f9',
          surface: '#FFFFFF',

          // Text
          textPrimary: '#0F172A',
          textSecondary: '#64748B',
          textMuted: '#94A3B8',

          // Borders
          border: '#CBD5E1',
          borderLight: '#E2E8F0',

          // States
          error: '#EF4444',
          errorBg: '#FEE2E2',
          success: '#22C55E',
          successBg: '#DCFCE7',
          warning: '#F59E0B',
          info: '#1E40AF'
        }
      }
    }
  },
  plugins: [],
}
