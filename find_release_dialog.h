#pragma once

#include <future>
#include <condition_variable>

#include "foo_discogs.h"
#include "resource.h"
#include "file_info_manager.h"
#include "multiformat.h"

using namespace Discogs;

class expand_master_release_process_callback;
class get_artist_process_callback;
class search_artist_process_callback;

enum class runHook { rhMaster, rhRelease, rhSubrelease, rhUndef};
enum class updRelSrc { Artist, Releases, Filter, Undef };



extern struct find_lv_config cfg_lv;

class CFindReleaseDialog : public MyCDialogImpl<CFindReleaseDialog>,
	public CDialogResize<CFindReleaseDialog>,
	public CMessageFilter
{
private:
	bool dropId = false;
	int dropId_ndx = -1;

	std::vector<std::thread> m_threads;
	std::mutex m_threadsMutex;
	bool m_finished_task{ true };
	std::mutex m_finishedMutex;
	std::condition_variable m_finishedCondVar;

	static void removeThread(CFindReleaseDialog* dlg, std::thread::id id)
	{
		std::lock_guard<std::mutex> lock(dlg->m_threadsMutex);
		auto iter = std::find_if(dlg->m_threads.begin(), dlg->m_threads.end(), [=](std::thread& t) { return (t.get_id() == id); });
		if (iter != dlg->m_threads.end())
		{
			iter->detach();
			dlg->m_threads.erase(iter);
		}
	}

	//consumer
	std::function<bool(CFindReleaseDialog* dlg, int list_index)> stdf_wait_consume =
		[this](CFindReleaseDialog* dlg, int list_index) -> bool {
			{
				std::unique_lock <std::mutex > lk{ m_finishedMutex };
				m_finishedCondVar.wait(lk, [dlg] {
					return dlg->m_finished_task;
					});
			}
			
			bool lvev = SendMessage(release_list, LVM_ENSUREVISIBLE,
				SendMessage(release_list, LVM_GETITEMCOUNT, 0, 0) - 1, TRUE); //Rool to the end
			SendMessage(release_list, LVM_ENSUREVISIBLE, list_index, TRUE); //Roll back to top position

			LPARAM lparam;
			get_item_param(release_list, list_index, 0, lparam);
			mounted_param myparam = get_mounted_param(lparam);

			pfc::string8 selected_id;
			if (myparam.bmaster && !myparam.brelease)
				selected_id = find_release_artist->master_releases[myparam.master_ndx]->main_release_id;
			else
			if (myparam.bmaster)
				selected_id = find_release_artist->master_releases[myparam.master_ndx]->sub_releases[myparam.release_ndx]->id;
			else
				selected_id = find_release_artist->releases[myparam.release_ndx]->id;

			uSetWindowText(release_url_edit, selected_id);

		return true; 
	};

	foo_discogs_conf conf;

	playable_location_impl location;
	file_info_impl info;

	metadb_handle_list items;

	titleformat_hook_impl_multiformat_ptr hook;

	pfc::array_t<Artist_ptr> artist_exact_matches;
	pfc::array_t<Artist_ptr> artist_other_matches;

	pfc::array_t<Artist_ptr> find_release_artists;
	Artist_ptr find_release_artist;

	std::vector<std::pair<int, int>> vec_icol_subitems;
	void update_sorted_icol_map(bool reset);

	void insert_item(const pfc::string8 &item, int list_index, int item_data);
	int insert_item_row(const std::list<std::pair<int, pfc::string8>>coldata, int list_index, int item_data);

	void reset_default_columns(bool reset);
	void get_item_text(HWND list, int list_index, int col, pfc::string8& out);
	void get_item_param(HWND list, int list_index, int col, LPARAM& out_p);
	void get_mounted_param(mounted_param& pm, LPARAM lparam);
	mounted_param get_mounted_param(LPARAM lparam);
	pfc::string8 run_hook_columns(std::list<std::pair<int, pfc::string8>>&col_data_list, runHook ismaster, runHook is_subrelease);

	HWND artist_list, release_list;
	HWND release_url_edit, search_edit, filter_edit;

	service_ptr_t<expand_master_release_process_callback> *active_task = nullptr;

	void load_size();
	bool build_current_cfg();
	void pushcfg();

	//TODO: add persistence to find release dialog columns

	void fill_artist_list();
	void select_release(int list_index_dropId = 0);
	void update_releases(const pfc::string8 &filter, updRelSrc updsrc);

	void search_artist();
	void on_search_artist_done(pfc::array_t<Artist_ptr> &p_artist_exact_matches, const pfc::array_t<Artist_ptr> &p_artist_other_matches);

	void get_selected_artist_releases();
	void on_get_artist_done(Artist_ptr &artist);

	void on_write_tags(const pfc::string8 &release_id);

	void expand_master_release(MasterRelease_ptr &master_release, int pos);
	void on_expand_master_release_done(const MasterRelease_ptr &master_release, int pos, threaded_process_status &p_status, abort_callback &p_abort);
	void on_expand_master_release_complete();

	void on_release_selected(int pos);

	void extract_id_from_url(pfc::string8 &s);
	void extract_release_from_link(pfc::string8 &s);

public:
	enum { IDD = IDD_DIALOG_FIND_RELEASE };
	//COMMAND_ID_HANDLER(IDC_FILTER_EDIT, OnEditFilterText)

#pragma warning( push )
#pragma warning( disable : 26454 )

	MY_BEGIN_MSG_MAP(CFindReleaseDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_PROCESS_RELEASE_BUTTON, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDC_RELEASE_URL_TEXT, OnEditReleaseIdText)
		COMMAND_ID_HANDLER(IDC_SEARCH_TEXT, OnEditSearchText)
		COMMAND_ID_HANDLER(IDC_FILTER_EDIT, OnEditFilterText)
		COMMAND_HANDLER(IDC_ARTIST_LIST, LBN_SELCHANGE, OnSelectArtist)
		COMMAND_HANDLER(IDC_ARTIST_LIST, LBN_DBLCLK, OnDoubleClickArtist)
		NOTIFY_HANDLER(IDC_RELEASE_LIST, /*LVN_ODSTATECHANGED*/LVN_ITEMCHANGED, OnListViewItemChanged)
		NOTIFY_HANDLER(IDC_RELEASE_LIST, NM_DBLCLK , OnDoubleClickRelease)
		NOTIFY_HANDLER(IDC_RELEASE_LIST, NM_CUSTOMDRAW, OnCustomDraw)
		COMMAND_ID_HANDLER(IDC_ONLY_EXACT_MATCHES_CHECK, OnCheckOnlyExactMatches)
		COMMAND_ID_HANDLER(IDC_SEARCH_BUTTON, OnSearch)
		COMMAND_ID_HANDLER(IDC_CLEAR_FILTER_BUTTON, OnClearFilter)
		COMMAND_ID_HANDLER(IDC_CONFIGURE_BUTTON, OnConfigure)
		CHAIN_MSG_MAP(CDialogResize<CFindReleaseDialog>)
	MY_END_MSG_MAP()

#pragma warning( pop )

	BEGIN_DLGRESIZE_MAP(CFindReleaseDialog)
		DLGRESIZE_CONTROL(IDC_LABEL_RELEASE_ID, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_RELEASE_URL_TEXT, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_FILTER_EDIT, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_RESULTS_GROUP, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_LABEL_FILTER, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_CLEAR_FILTER_BUTTON, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_ARTIST_LIST, DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_RELEASE_LIST, DLSZ_SIZE_Y)
		BEGIN_DLGRESIZE_GROUP()
			DLGRESIZE_CONTROL(IDC_ARTIST_LIST, DLSZ_SIZE_X)
			DLGRESIZE_CONTROL(IDC_RELEASE_LIST, DLSZ_SIZE_X)
			DLGRESIZE_CONTROL(IDC_LABEL_RELEASES, DLSZ_MOVE_X)
		END_DLGRESIZE_GROUP()
		DLGRESIZE_CONTROL(IDC_ONLY_EXACT_MATCHES_CHECK, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CONFIGURE_BUTTON, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_PROCESS_RELEASE_BUTTON, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	void DlgResize_UpdateLayout(int cxWidth, int cyHeight) {
		CDialogResize<CFindReleaseDialog>::DlgResize_UpdateLayout(cxWidth, cyHeight);
	}

	CFindReleaseDialog(HWND p_parent, metadb_handle_list items, bool dropId) : items(items), dropId(dropId) {
		find_release_artist = nullptr;
		Create(p_parent);
	};
	~CFindReleaseDialog();
	
	void OnFinalMessage(HWND /*hWnd*/) override;

	virtual BOOL PreTranslateMessage(MSG* pMsg) override {
		return ::IsDialogMessage(m_hWnd, pMsg);
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEditReleaseIdText(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditSearchText(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditFilterText(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSelectArtist(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDoubleClickArtist(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCustomDraw(int wParam, LPNMHDR lParam, BOOL bHandled);
	LRESULT OnListViewItemChanged(int, LPNMHDR hdr, BOOL&);
	LRESULT OnDoubleClickRelease(int, LPNMHDR hdr, BOOL&);
	LRESULT OnCheckOnlyExactMatches(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSearch(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClearFilter(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnConfigure(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	friend class expand_master_release_process_callback;
	friend class get_artist_process_callback;
	friend class search_artist_process_callback;

	void enable(bool is_enabled) override;
};
