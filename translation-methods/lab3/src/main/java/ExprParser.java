// Generated from Expr.g4 by ANTLR 4.9.2

import java.util.HashMap;
import java.util.Map;

import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class ExprParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.9.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		WS=1, PLUS=2, MINUS=3, ASTER=4, SLASH=5, SLASH2=6, LPAREN=7, RPAREN=8, 
		EQ=9, SEMICOLON=10, VAR=11, NUM=12;
	public static final int
		RULE_factor = 0, RULE_logexp = 1, RULE_term = 2, RULE_term1 = 3, RULE_expr = 4, 
		RULE_expr1 = 5, RULE_assign = 6, RULE_parse = 7;
	private static String[] makeRuleNames() {
		return new String[] {
			"factor", "logexp", "term", "term1", "expr", "expr1", "assign", "parse"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "WS", "PLUS", "MINUS", "ASTER", "SLASH", "SLASH2", "LPAREN", "RPAREN", 
			"EQ", "SEMICOLON", "VAR", "NUM"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "Expr.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }


	Map<String, Integer> memory = new HashMap<>();
	StringBuilder sb = new StringBuilder();

	public ExprParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	public static class FactorContext extends ParserRuleContext {
		public Integer res;
		public ExprContext expr;
		public FactorContext factor;
		public Token VAR;
		public Token NUM;
		public TerminalNode LPAREN() { return getToken(ExprParser.LPAREN, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public TerminalNode RPAREN() { return getToken(ExprParser.RPAREN, 0); }
		public TerminalNode MINUS() { return getToken(ExprParser.MINUS, 0); }
		public FactorContext factor() {
			return getRuleContext(FactorContext.class,0);
		}
		public TerminalNode VAR() { return getToken(ExprParser.VAR, 0); }
		public TerminalNode NUM() { return getToken(ExprParser.NUM, 0); }
		public FactorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_factor; }
	}

	public final FactorContext factor() throws RecognitionException {
		FactorContext _localctx = new FactorContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_factor);
		try {
			setState(29);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case LPAREN:
				enterOuterAlt(_localctx, 1);
				{
				setState(16);
				match(LPAREN);
				setState(17);
				((FactorContext)_localctx).expr = expr();
				setState(18);
				match(RPAREN);
				 ((FactorContext)_localctx).res =  ((FactorContext)_localctx).expr.res; 
				}
				break;
			case MINUS:
				enterOuterAlt(_localctx, 2);
				{
				setState(21);
				match(MINUS);
				setState(22);
				((FactorContext)_localctx).factor = factor();
				 ((FactorContext)_localctx).res =  -((FactorContext)_localctx).factor.res; 
				}
				break;
			case VAR:
				enterOuterAlt(_localctx, 3);
				{
				setState(25);
				((FactorContext)_localctx).VAR = match(VAR);
				 ((FactorContext)_localctx).res =  memory.get((((FactorContext)_localctx).VAR!=null?((FactorContext)_localctx).VAR.getText():null)); 
				}
				break;
			case NUM:
				enterOuterAlt(_localctx, 4);
				{
				setState(27);
				((FactorContext)_localctx).NUM = match(NUM);
				 ((FactorContext)_localctx).res =  Integer.parseInt((((FactorContext)_localctx).NUM!=null?((FactorContext)_localctx).NUM.getText():null)); 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class LogexpContext extends ParserRuleContext {
		public Integer res;
		public FactorContext factor;
		public LogexpContext logexp;
		public FactorContext factor() {
			return getRuleContext(FactorContext.class,0);
		}
		public TerminalNode SLASH2() { return getToken(ExprParser.SLASH2, 0); }
		public LogexpContext logexp() {
			return getRuleContext(LogexpContext.class,0);
		}
		public LogexpContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_logexp; }
	}

	public final LogexpContext logexp() throws RecognitionException {
		LogexpContext _localctx = new LogexpContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_logexp);
		try {
			setState(39);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,1,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(31);
				((LogexpContext)_localctx).factor = factor();
				setState(32);
				match(SLASH2);
				setState(33);
				((LogexpContext)_localctx).logexp = logexp();
				 ((LogexpContext)_localctx).res =  (int) Math.round(Math.log(((LogexpContext)_localctx).factor.res) / Math.log(((LogexpContext)_localctx).logexp.res)); 
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(36);
				((LogexpContext)_localctx).factor = factor();
				 ((LogexpContext)_localctx).res =  ((LogexpContext)_localctx).factor.res; 
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TermContext extends ParserRuleContext {
		public Integer res;
		public LogexpContext logexp;
		public Term1Context term1;
		public LogexpContext logexp() {
			return getRuleContext(LogexpContext.class,0);
		}
		public Term1Context term1() {
			return getRuleContext(Term1Context.class,0);
		}
		public TermContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_term; }
	}

	public final TermContext term() throws RecognitionException {
		TermContext _localctx = new TermContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_term);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(41);
			((TermContext)_localctx).logexp = logexp();
			setState(42);
			((TermContext)_localctx).term1 = term1(((TermContext)_localctx).logexp.res);
			 ((TermContext)_localctx).res =  ((TermContext)_localctx).term1.res; 
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Term1Context extends ParserRuleContext {
		public Integer lhs;
		public Integer res;
		public Token op;
		public LogexpContext logexp;
		public Term1Context term1;
		public LogexpContext logexp() {
			return getRuleContext(LogexpContext.class,0);
		}
		public Term1Context term1() {
			return getRuleContext(Term1Context.class,0);
		}
		public TerminalNode ASTER() { return getToken(ExprParser.ASTER, 0); }
		public TerminalNode SLASH() { return getToken(ExprParser.SLASH, 0); }
		public Term1Context(ParserRuleContext parent, int invokingState) { super(parent, invokingState); }
		public Term1Context(ParserRuleContext parent, int invokingState, Integer lhs) {
			super(parent, invokingState);
			this.lhs = lhs;
		}
		@Override public int getRuleIndex() { return RULE_term1; }
	}

	public final Term1Context term1(Integer lhs) throws RecognitionException {
		Term1Context _localctx = new Term1Context(_ctx, getState(), lhs);
		enterRule(_localctx, 6, RULE_term1);
		int _la;
		try {
			setState(51);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case ASTER:
			case SLASH:
				enterOuterAlt(_localctx, 1);
				{
				setState(45);
				((Term1Context)_localctx).op = _input.LT(1);
				_la = _input.LA(1);
				if ( !(_la==ASTER || _la==SLASH) ) {
					((Term1Context)_localctx).op = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(46);
				((Term1Context)_localctx).logexp = logexp();
				setState(47);
				((Term1Context)_localctx).term1 = term1((((Term1Context)_localctx).op!=null?((Term1Context)_localctx).op.getText():null).endsWith("*") ? _localctx.lhs * ((Term1Context)_localctx).logexp.res : _localctx.lhs / ((Term1Context)_localctx).logexp.res);
				 ((Term1Context)_localctx).res =  ((Term1Context)_localctx).term1.res; 
				}
				break;
			case PLUS:
			case MINUS:
			case RPAREN:
			case SEMICOLON:
				enterOuterAlt(_localctx, 2);
				{
				 ((Term1Context)_localctx).res =  _localctx.lhs; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ExprContext extends ParserRuleContext {
		public Integer res;
		public TermContext term;
		public Expr1Context expr1;
		public TermContext term() {
			return getRuleContext(TermContext.class,0);
		}
		public Expr1Context expr1() {
			return getRuleContext(Expr1Context.class,0);
		}
		public ExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expr; }
	}

	public final ExprContext expr() throws RecognitionException {
		ExprContext _localctx = new ExprContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_expr);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(53);
			((ExprContext)_localctx).term = term();
			setState(54);
			((ExprContext)_localctx).expr1 = expr1(((ExprContext)_localctx).term.res);
			 ((ExprContext)_localctx).res =  ((ExprContext)_localctx).expr1.res; 
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Expr1Context extends ParserRuleContext {
		public Integer lhs;
		public Integer res;
		public Token op;
		public TermContext term;
		public Expr1Context expr1;
		public TermContext term() {
			return getRuleContext(TermContext.class,0);
		}
		public Expr1Context expr1() {
			return getRuleContext(Expr1Context.class,0);
		}
		public TerminalNode PLUS() { return getToken(ExprParser.PLUS, 0); }
		public TerminalNode MINUS() { return getToken(ExprParser.MINUS, 0); }
		public Expr1Context(ParserRuleContext parent, int invokingState) { super(parent, invokingState); }
		public Expr1Context(ParserRuleContext parent, int invokingState, Integer lhs) {
			super(parent, invokingState);
			this.lhs = lhs;
		}
		@Override public int getRuleIndex() { return RULE_expr1; }
	}

	public final Expr1Context expr1(Integer lhs) throws RecognitionException {
		Expr1Context _localctx = new Expr1Context(_ctx, getState(), lhs);
		enterRule(_localctx, 10, RULE_expr1);
		int _la;
		try {
			setState(63);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case PLUS:
			case MINUS:
				enterOuterAlt(_localctx, 1);
				{
				setState(57);
				((Expr1Context)_localctx).op = _input.LT(1);
				_la = _input.LA(1);
				if ( !(_la==PLUS || _la==MINUS) ) {
					((Expr1Context)_localctx).op = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(58);
				((Expr1Context)_localctx).term = term();
				setState(59);
				((Expr1Context)_localctx).expr1 = expr1((((Expr1Context)_localctx).op!=null?((Expr1Context)_localctx).op.getText():null).endsWith("+") ? _localctx.lhs + ((Expr1Context)_localctx).term.res : _localctx.lhs - ((Expr1Context)_localctx).term.res);
				 ((Expr1Context)_localctx).res =  ((Expr1Context)_localctx).expr1.res; 
				}
				break;
			case RPAREN:
			case SEMICOLON:
				enterOuterAlt(_localctx, 2);
				{
				 ((Expr1Context)_localctx).res =  _localctx.lhs; 
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AssignContext extends ParserRuleContext {
		public Token VAR;
		public ExprContext expr;
		public TerminalNode VAR() { return getToken(ExprParser.VAR, 0); }
		public TerminalNode EQ() { return getToken(ExprParser.EQ, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public TerminalNode SEMICOLON() { return getToken(ExprParser.SEMICOLON, 0); }
		public AssignContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_assign; }
	}

	public final AssignContext assign() throws RecognitionException {
		AssignContext _localctx = new AssignContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_assign);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(65);
			((AssignContext)_localctx).VAR = match(VAR);
			setState(66);
			match(EQ);
			setState(67);
			((AssignContext)_localctx).expr = expr();
			setState(68);
			match(SEMICOLON);

			memory.put((((AssignContext)_localctx).VAR!=null?((AssignContext)_localctx).VAR.getText():null), ((AssignContext)_localctx).expr.res);
			sb.append((((AssignContext)_localctx).VAR!=null?((AssignContext)_localctx).VAR.getText():null) + " = " + ((AssignContext)_localctx).expr.res + "\n");

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ParseContext extends ParserRuleContext {
		public TerminalNode EOF() { return getToken(ExprParser.EOF, 0); }
		public List<AssignContext> assign() {
			return getRuleContexts(AssignContext.class);
		}
		public AssignContext assign(int i) {
			return getRuleContext(AssignContext.class,i);
		}
		public List<TerminalNode> WS() { return getTokens(ExprParser.WS); }
		public TerminalNode WS(int i) {
			return getToken(ExprParser.WS, i);
		}
		public ParseContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_parse; }
	}

	public final ParseContext parse() throws RecognitionException {
		ParseContext _localctx = new ParseContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_parse);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(74);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==VAR) {
				{
				{
				setState(71);
				assign();
				}
				}
				setState(76);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(80);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==WS) {
				{
				{
				setState(77);
				match(WS);
				}
				}
				setState(82);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(83);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3\16X\4\2\t\2\4\3\t"+
		"\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\3\2\3\2\3\2\3\2\3\2"+
		"\3\2\3\2\3\2\3\2\3\2\3\2\3\2\3\2\5\2 \n\2\3\3\3\3\3\3\3\3\3\3\3\3\3\3"+
		"\3\3\5\3*\n\3\3\4\3\4\3\4\3\4\3\5\3\5\3\5\3\5\3\5\3\5\5\5\66\n\5\3\6\3"+
		"\6\3\6\3\6\3\7\3\7\3\7\3\7\3\7\3\7\5\7B\n\7\3\b\3\b\3\b\3\b\3\b\3\b\3"+
		"\t\7\tK\n\t\f\t\16\tN\13\t\3\t\7\tQ\n\t\f\t\16\tT\13\t\3\t\3\t\3\t\2\2"+
		"\n\2\4\6\b\n\f\16\20\2\4\3\2\6\7\3\2\4\5\2W\2\37\3\2\2\2\4)\3\2\2\2\6"+
		"+\3\2\2\2\b\65\3\2\2\2\n\67\3\2\2\2\fA\3\2\2\2\16C\3\2\2\2\20L\3\2\2\2"+
		"\22\23\7\t\2\2\23\24\5\n\6\2\24\25\7\n\2\2\25\26\b\2\1\2\26 \3\2\2\2\27"+
		"\30\7\5\2\2\30\31\5\2\2\2\31\32\b\2\1\2\32 \3\2\2\2\33\34\7\r\2\2\34 "+
		"\b\2\1\2\35\36\7\16\2\2\36 \b\2\1\2\37\22\3\2\2\2\37\27\3\2\2\2\37\33"+
		"\3\2\2\2\37\35\3\2\2\2 \3\3\2\2\2!\"\5\2\2\2\"#\7\b\2\2#$\5\4\3\2$%\b"+
		"\3\1\2%*\3\2\2\2&\'\5\2\2\2\'(\b\3\1\2(*\3\2\2\2)!\3\2\2\2)&\3\2\2\2*"+
		"\5\3\2\2\2+,\5\4\3\2,-\5\b\5\2-.\b\4\1\2.\7\3\2\2\2/\60\t\2\2\2\60\61"+
		"\5\4\3\2\61\62\5\b\5\2\62\63\b\5\1\2\63\66\3\2\2\2\64\66\b\5\1\2\65/\3"+
		"\2\2\2\65\64\3\2\2\2\66\t\3\2\2\2\678\5\6\4\289\5\f\7\29:\b\6\1\2:\13"+
		"\3\2\2\2;<\t\3\2\2<=\5\6\4\2=>\5\f\7\2>?\b\7\1\2?B\3\2\2\2@B\b\7\1\2A"+
		";\3\2\2\2A@\3\2\2\2B\r\3\2\2\2CD\7\r\2\2DE\7\13\2\2EF\5\n\6\2FG\7\f\2"+
		"\2GH\b\b\1\2H\17\3\2\2\2IK\5\16\b\2JI\3\2\2\2KN\3\2\2\2LJ\3\2\2\2LM\3"+
		"\2\2\2MR\3\2\2\2NL\3\2\2\2OQ\7\3\2\2PO\3\2\2\2QT\3\2\2\2RP\3\2\2\2RS\3"+
		"\2\2\2SU\3\2\2\2TR\3\2\2\2UV\7\2\2\3V\21\3\2\2\2\b\37)\65ALR";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}