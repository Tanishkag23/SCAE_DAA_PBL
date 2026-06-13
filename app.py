import streamlit as st

# ──────────────────────────────────────────────────────────────
# Page config — sidebar collapsed by default (login page hai)
# ──────────────────────────────────────────────────────────────
st.set_page_config(
    page_title="SCAE — Smart City Portal",
    page_icon="🏙️",
    layout="wide",
    initial_sidebar_state="collapsed",
)

# Default Streamlit sidebar nav chhupa do — apna role-based nav use karenge
st.markdown(
    "<style>[data-testid='stSidebarNav'] {display: none;}</style>",
    unsafe_allow_html=True,
)

# ──────────────────────────────────────────────────────────────
# Session state initialise karo
# ──────────────────────────────────────────────────────────────
if "role" not in st.session_state:
    st.session_state.role = None
if "user_name" not in st.session_state:
    st.session_state.user_name = ""

# ──────────────────────────────────────────────────────────────
# Center layout — teeno columns mein beech wali use karenge
# ──────────────────────────────────────────────────────────────
_, col2, _ = st.columns([1, 2, 1])

with col2:
    # Hero section
    st.markdown(
        "<h1 style='text-align:center; font-size: 3rem;'>🏙️ Smart City Portal</h1>",
        unsafe_allow_html=True,
    )
    st.markdown(
        "<p style='text-align:center; color:#888; font-size:1.1rem;'>"
        "Powered by <b>SCAE</b> — Smart City Algorithm Engine"
        "</p>",
        unsafe_allow_html=True,
    )

    st.divider()

    # Name input
    name = st.text_input("👤 Enter your name", placeholder="e.g. Ramesh Kumar", key="login_name")
    st.session_state.user_name = name

    st.write("**Select your role:**")

    b1, b2, b3 = st.columns(3)

    # ── Citizen ──────────────────────────────────────────────
    with b1:
        if st.button("👤 Citizen", use_container_width=True):
            if not st.session_state.user_name.strip():
                st.warning("⚠️ Please enter your name first!")
            else:
                st.session_state.role = "citizen"
                st.switch_page("pages/01_citizen_dashboard.py")

    # ── Operator ─────────────────────────────────────────────
    with b2:
        if st.button("🚦 Operator", use_container_width=True):
            if not st.session_state.user_name.strip():
                st.warning("⚠️ Please enter your name first!")
            else:
                st.session_state.role = "operator"
                st.switch_page("pages/02_operator_dashboard.py")

    # ── Admin ─────────────────────────────────────────────────
    with b3:
        if st.button("🛠️ Admin", use_container_width=True):
            if not st.session_state.user_name.strip():
                st.warning("⚠️ Please enter your name first!")
            else:
                st.session_state.role = "admin"
                st.switch_page("pages/03_admin_dashboard.py")

    st.divider()

    st.caption(
        "35 algorithms running behind the scenes — "
        "powered by C + ctypes + Streamlit"
    )
