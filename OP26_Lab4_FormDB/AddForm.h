#pragma once

namespace OP26Lab4FormDB {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace MySql::Data::MySqlClient;

	public ref class AddForm : public System::Windows::Forms::Form
	{
	public:
		AddForm(void)
		{
			InitializeComponent();
			InitializeCustomControls();
		}

	protected:
		~AddForm() { if (components) delete components; }

	private:
		TextBox^ txtType;
		TextBox^ txtBrand;
		TextBox^ txtManuf;
		DateTimePicker^ dtpDate;
		TextBox^ txtSupplier;
		TextBox^ txtPrice;
		Button^ btnSave;
		System::ComponentModel::Container ^components;

		void InitializeCustomControls() {
			this->Text = L"Додавання товару";
			this->Size = System::Drawing::Size(400, 480);
			this->StartPosition = FormStartPosition::CenterParent;

			CreateField(L"Вид (пиво, сидр):", 20, txtType);
			CreateField(L"Марка:", 70, txtBrand);
			CreateField(L"Виробник:", 120, txtManuf);
			
			Label^ l = gcnew Label(); l->Text = L"Термін придатності:"; l->Location = Point(20, 170); l->AutoSize = true;
			this->Controls->Add(l);
			dtpDate = gcnew DateTimePicker(); dtpDate->Location = Point(20, 190); dtpDate->Size = System::Drawing::Size(340, 30);
			dtpDate->Format = DateTimePickerFormat::Short;
			this->Controls->Add(dtpDate);

			CreateField(L"Постачальник:", 220, txtSupplier);
			CreateField(L"Ціна (грн):", 270, txtPrice);

			btnSave = gcnew Button();
			btnSave->Text = L"Зберегти в БД";
			btnSave->Location = Point(100, 350);
			btnSave->Size = System::Drawing::Size(180, 40);
			btnSave->Click += gcnew EventHandler(this, &AddForm::OnSaveClick);
			this->Controls->Add(btnSave);
		}

		void CreateField(String^ lblText, int y, TextBox^% txtBox) {
			Label^ l = gcnew Label(); l->Text = lblText; l->Location = Point(20, y); l->AutoSize = true;
			this->Controls->Add(l);
			txtBox = gcnew TextBox(); txtBox->Location = Point(20, y + 20); txtBox->Size = System::Drawing::Size(340, 20);
			this->Controls->Add(txtBox);
		}

		void OnSaveClick(Object^ sender, EventArgs^ e) {
			if (txtType->Text == "" || txtBrand->Text == "" || txtPrice->Text == "") {
				MessageBox::Show(L"Будь ласка, заповніть основні поля (Вид, Марка, Ціна)!");
				return;
			}

			String^ connString = "Server=localhost;Database=shop_db;Uid=root;Pwd=V30121995;Charset=utf8;";
			MySqlConnection^ conn = gcnew MySqlConnection(connString);

			try {
				conn->Open();
				String^ sql = "INSERT INTO products (type, brand, manufacturer, expiration_date, supplier, price) VALUES (@t, @b, @m, @d, @s, @p)";
				MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);

				cmd->Parameters->AddWithValue("@t", txtType->Text);
				cmd->Parameters->AddWithValue("@b", txtBrand->Text);
				cmd->Parameters->AddWithValue("@m", txtManuf->Text);
				cmd->Parameters->AddWithValue("@d", dtpDate->Value);
				cmd->Parameters->AddWithValue("@s", txtSupplier->Text);
				// Конвертуємо ціну, замінюючи кому на крапку, якщо треба
				String^ priceStr = txtPrice->Text->Replace(",", ".");
				cmd->Parameters->AddWithValue("@p", Convert::ToDecimal(priceStr));

				cmd->ExecuteNonQuery();
				
				MessageBox::Show(L"Товар успішно додано!");
				this->Close(); 
			}
			catch (Exception^ ex) {
				MessageBox::Show(L"Помилка: " + ex->Message);
			}
			finally {
				conn->Close();
			}
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