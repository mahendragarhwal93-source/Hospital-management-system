import streamlit as st
import pandas as pd
from datetime import datetime
import os

FILE_NAME = "Hospital.csv"
BACKUP_FILE = "Hospital_backup.csv"

# ---------------------------
# Utility Functions
# ---------------------------
def load_patients():
    if not os.path.exists(FILE_NAME):
        return pd.DataFrame(columns=["Name", "Address", "Blood Group", "Admission Date"])
    return pd.read_csv(FILE_NAME)

def save_patients(df):
    df.to_csv(FILE_NAME, index=False)

def backup_file():
    if os.path.exists(FILE_NAME):
        df = pd.read_csv(FILE_NAME)
        df.to_csv(BACKUP_FILE, index=False)

# ---------------------------
# Authentication
# ---------------------------
def login():
    st.title("🏥 Hospital Management Login")
    username = st.text_input("Username")
    password = st.text_input("Password", type="password")
    if st.button("Login"):
        if username == "mahendra" and password == "1234":
            st.session_state["authenticated"] = True
            st.success("Login successful!")
        else:
            st.error("Invalid credentials")

# ---------------------------
# Main Interface
# ---------------------------
def main_app():
    st.sidebar.title("🏥 Hospital Management System")
    menu = st.sidebar.radio("Menu", ["Admit Patient", "Discharge Patient", "Show All Patients", "Search Patient", "Logout"])

    df = load_patients()

    if menu == "Admit Patient":
        st.header("➕ Admit New Patient")
        name = st.text_input("Patient Name")
        address = st.text_input("Address")
        bgroup = st.text_input("Blood Group")
        if st.button("Admit"):
            if name and address and bgroup:
                date = datetime.now().strftime("%d/%m/%Y")
                new_patient = pd.DataFrame([[name, address, bgroup, date]], columns=df.columns)
                df = pd.concat([df, new_patient], ignore_index=True)
                save_patients(df)
                st.success(f"Patient '{name}' admitted successfully on {date}!")
            else:
                st.warning("Please fill all fields.")

    elif menu == "Discharge Patient":
        st.header("🩹 Discharge Patient")
        backup_file()
        if not df.empty:
            patient_name = st.selectbox("Select Patient to Discharge", df["Name"])
            if st.button("Discharge"):
                df = df[df["Name"] != patient_name]
                save_patients(df)
                st.success(f"Patient '{patient_name}' discharged successfully!")
        else:
            st.info("No patients currently admitted.")

    elif menu == "Show All Patients":
        st.header("📋 All Admitted Patients")
        if not df.empty:
            st.dataframe(df)
        else:
            st.info("No patients currently admitted.")

    elif menu == "Search Patient":
        st.header("🔍 Search Patient")
        search_by = st.radio("Search by:", ["Name", "Blood Group"])
        query = st.text_input(f"Enter {search_by}")
        if st.button("Search"):
            if search_by == "Name":
                results = df[df["Name"].str.lower() == query.lower()]
            else:
                results = df[df["Blood Group"].str.lower() == query.lower()]
            if not results.empty:
                st.dataframe(results)
            else:
                st.warning("No matching records found.")

    elif menu == "Logout":
        st.session_state["authenticated"] = False
        st.success("Logged out successfully!")

# ---------------------------
# Main Program
# ---------------------------
if "authenticated" not in st.session_state:
    st.session_state["authenticated"] = False

if not st.session_state["authenticated"]:
    login()
else:
    main_app()
