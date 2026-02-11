#pragma once
#include "AddForm.h" 

namespace OP26Lab4FormDB {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace MySql::Data::MySqlClient;

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			InitializeCustomControls();
			LoadData(""); 
		}

	protected:
		~MainForm() { if (components) delete components; }

	private:
		DataGridView^ dgv;
		Button^ btnAdd;
		Button^ btnDelete;
		Button^ btnRefresh;
		TextBox^ txtSearch;
		Label^ lblSearch;
		System::ComponentModel::Container^ components;

		void InitializeCustomControls() {
			this->Text = L"Облік слабоалкогольних напоїв";
			this->Size = System::Drawing::Size(950, 600);
			this->StartPosition = FormStartPosition::CenterScreen;

			btnAdd = gcnew Button();
			btnAdd->Text = L"Додати товар";
			btnAdd->Location = Point(12, 12);
			btnAdd->Size = System::Drawing::Size(150, 40);
			btnAdd->Click += gcnew EventHandler(this, &MainForm::OnAddClick);
			this->Controls->Add(btnAdd);

			btnDelete = gcnew Button();
			btnDelete->Text = L"Видалити обране";
			btnDelete->Location = Point(170, 12);
			btnDelete->Size = System::Drawing::Size(150, 40);
			btnDelete->Click += gcnew EventHandler(this, &MainForm::OnDeleteClick);
			this->Controls->Add(btnDelete);

			btnRefresh = gcnew Button();
			btnRefresh->Text = L"Оновити";
			btnRefresh->Location = Point(330, 12);
			btnRefresh->Size = System::Drawing::Size(100, 40);
			btnRefresh->Click += gcnew EventHandler(this, &MainForm::OnRefreshClick);
			this->Controls->Add(btnRefresh);

			lblSearch = gcnew Label();
			lblSearch->Text = L"Пошук за назвою:";
			lblSearch->Location = Point(450, 25);
			lblSearch->AutoSize = true;
			this->Controls->Add(lblSearch);

			txtSearch = gcnew TextBox();
			txtSearch->Location = Point(560, 22);
			txtSearch->Size = System::Drawing::Size(200, 30);
			txtSearch->TextChanged += gcnew EventHandler(this, &MainForm::OnSearchChange);
			this->Controls->Add(txtSearch);

			dgv = gcnew DataGridView();
			dgv->Location = Point(12, 70);
			dgv->Size = System::Drawing::Size(910, 480);
			dgv->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
			dgv->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
			dgv->ReadOnly = true;
			this->Controls->Add(dgv);
		}

		void LoadData(String^ searchText) {
			String^ connString = "Server=localhost;Database=shop_db;Uid=root;Pwd=V30121995;Charset=utf8;";
			MySqlConnection^ conn = gcnew MySqlConnection(connString);

			try {
				conn->Open();
				String^ sql = "";
				MySqlCommand^ cmd;

				if (String::IsNullOrEmpty(searchText)) {
					sql = "SELECT * FROM products";
					cmd = gcnew MySqlCommand(sql, conn);
				}
				else {
					sql = "SELECT * FROM products WHERE brand LIKE @search OR type LIKE @search";
					cmd = gcnew MySqlCommand(sql, conn);
					cmd->Parameters->AddWithValue("@search", "%" + searchText + "%");
				}

				MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(cmd);
				DataTable^ table = gcnew DataTable();
				adapter->Fill(table);
				dgv->DataSource = table;
			}
			catch (Exception^ ex) {
				MessageBox::Show(L"Помилка: " + ex->Message);
			}
			finally {
				conn->Close();
			}
		}

		void OnAddClick(Object^ sender, EventArgs^ e) {
			AddForm^ form = gcnew AddForm();
			form->ShowDialog();
			LoadData("");
		}

		void OnRefreshClick(Object^ sender, EventArgs^ e) {
			LoadData("");
		}

		void OnSearchChange(Object^ sender, EventArgs^ e) {
			LoadData(txtSearch->Text);
		}

		void OnDeleteClick(Object^ sender, EventArgs^ e) {
			if (dgv->SelectedRows->Count > 0) {
				String^ id = dgv->SelectedRows[0]->Cells[0]->Value->ToString();

				if (MessageBox::Show(L"Видалити запис ID: " + id + "?",
					L"Підтвердження", MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == System::Windows::Forms::DialogResult::Yes) {

					DeleteRecord(id);
					LoadData("");
				}
			}
			else {
				MessageBox::Show(L"Оберіть рядок!");
			}
		}

		void DeleteRecord(String^ id) {
			String^ connString = "Server=localhost;Database=shop_db;Uid=root;Pwd=V30121995;";
			MySqlConnection^ conn = gcnew MySqlConnection(connString);
			try {
				conn->Open();
				MySqlCommand^ cmd = gcnew MySqlCommand("DELETE FROM products WHERE id = @id", conn);
				cmd->Parameters->AddWithValue("@id", id);
				cmd->ExecuteNonQuery();
			}
			catch (Exception^ ex) {
				MessageBox::Show(L"Помилка видалення: " + ex->Message);
			}
			conn->Close();
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		}
#pragma endregion
	};
}